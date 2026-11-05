# Lifecycle

This page describes what happens from the moment the engine process starts to the moment it exits, and where Lua scripts are invoked.

All statements here reflect the C++ implementation (`main.cpp`, `Engine.cpp`, `SceneManager.cpp`, `LuaComponent.cpp`). Anything not shown there is not engine behavior.

## Process lifecycle

The engine has exactly three top-level steps, executed in `main.cpp`:

    if (init() != 0) return 1;
    run();
    quit();

### 1. `init()` — engine initialization

Creates the window, renderers, and subsystems. **No Lua code runs during `init()`.**

Order:

1. Read `config.json`. If missing or unreadable, a default config is written and used.
2. `SDL_Init(VIDEO | AUDIO)`.
3. `IMG_Init(PNG)`.
4. `initTTF()`.
5. Create the window (OpenGL context if `renderer` is `"opengl"`; fullscreen desktop if `fullscreen` is true).
6. Load the window icon (BMP). Failure is a warning, not fatal.
7. Create the `SDL_Renderer`.
8. Create the texture backend (`GLTextureBackend` or `SDLTextureBackend`) and the renderer (`OpenGLRenderer` or `SDLRenderer`).

`init()` returns `0` on success. Non-zero values indicate which step failed:

| Return | Failed step |
|--------|-------------|
| 1 | `SDL_Init` |
| 2 | `IMG_Init` |
| 3 | TTF initialization |
| 4 | Window creation |
| 5 | Renderer creation |

### 2. `run()` — main loop

`run()` performs setup and then loops until the engine is asked to quit.

**Setup, in order:**

1. `registerComponents()` — registers component factories.
2. `Timer::initTimer()`.
3. `Lua::init()` — creates the global Lua state and registers all Lua namespaces and types (see [Engine Overview](engine.md)).
4. `SceneManager::loadScene("home")` — loads `Scenes/home.json`.

**Each frame, in order:**

1. `Input::BeginFrame()` then `Input::Update()`.
2. If a quit has been requested (`Input::QuitRequested()`), the loop ends.
3. If the window was resized, the renderer is resized.
4. `dt = Timer::deltaTime()`.
5. Box colliders are synced into the physics world.
6. `physics->updatePhysics(dt)` and `physics->collectEvents()`.
7. Each physics event is delivered:
   - First to the affected `GameObject`'s C++ components via `OnCollisionEnter` / `OnCollisionExit` / `OnTriggerEnter` / `OnTriggerExit`.
   - Then, if the `GameObject` has a Lua callback set (`onCollisionEnter`, `onCollisionExit`, `onTriggerEnter`, `onTriggerExit` properties on the Lua `GameObject`), that function is called with a `GameObject` handle for the other object.
8. For every object in the current scene: `obj->Update(dt)`. This calls `Update(dt)` on each component. **This is where `LuaComponent::Update` runs your Lua `start()` (once) and `update(dt)` (every frame).**
9. `UIManager::Update()`.
10. Rendering: `beginFrame()`, `drawScene(...)`, `endFrame()`. In `DEV_MODE`, physics debug drawing and the log overlay are also rendered.
11. `SceneManager::processPendingScene()` — applies any scene switch requested during the frame.

### 3. `quit()` — shutdown

Runs once after the loop ends:

1. `FontManager::clean()`.
2. Clear the in-game log.
3. Release texture backend and renderer backend.
4. `SDL_DestroyRenderer`, `SDL_DestroyWindow`.

Lua `onDestroy` functions on attached `LuaComponent`s run when their scene is unloaded or their object is destroyed, not during `quit()`.

## Scene lifecycle

Scenes are loaded from `Scenes/<name>.json` by `SceneManager::loadScene`. `run()` loads `home` at startup.

`loadScene(name)`:

1. Calls `onExit()` on the current scene (which calls `OnDestroy` on every component of every object).
2. Clears the object list and the ID lookup table.
3. Resets the physics world.
4. Reads `Scenes/<name>.json`. Failure is logged; the function returns without loading.
5. Reads optional `physics.gravity` (default `{0, -9.8}`) and creates the physics world.
6. Reads the `objects` array. Missing `objects` is an error.
7. Recursively loads each object: name, ID, tag, transform, components, children.
8. Calls `onEnter()` on the new scene (currently a no-op).

### Scene switching

Calling `Scene.load(name)` from Lua does **not** switch immediately. It records a pending scene name. `run()` applies it at the end of the frame via `SceneManager::processPendingScene()`. This means the current frame finishes cleanly before the old scene is torn down.

## Object and component lifecycle

### GameObject creation

`Scene.createObject(name)` (from Lua `Objects.create`) or a scene file creates a `GameObject` with a fresh ID, a default `Transform`, and no components.

### Component addition

`obj->addComponent(comp)`:

1. Sets `comp->owner`.
2. If a component of the same `typeName()` already exists, calls `OnDestroy()` on the old one and removes it.
3. Inserts the new component.
4. Calls `OnCreate()` on the new component.

For Lua components specifically, `awake()` is called **before** `DeSerialize` when loading from a scene file, and `OnCreate()` is a no-op.

### Per-frame update

`GameObject::Update(dt)` iterates over its components and calls `comp->Update(dt)`. There is no separate per-object script update; all Lua logic runs through the `LuaComponent`'s `Update`.

### Destruction

`SceneManager::deleteObjectById` (from Lua `Objects.deleteById` or `handle:destroy()`):

1. Calls `detachFromHierarchy()` on the object.
2. Calls `OnDestroy()` on every component.
3. Removes the object from the scene and the ID lookup.

Destroying all objects by name or tag uses the same destruction path per object.

## Lua script lifecycle (`LuaComponent`)

A `LuaComponent` is created when a scene JSON object has a key of the form `"lua:<path>"` (for example, `"lua:Scripts/player.lua": {}`). Its `typeName()` is `"lua:" + scriptPath`.

When the scene is loaded, for each Lua component:

1. **`awake()`** runs. It creates a fresh Lua environment with the global table as fallback, sets `owner` in that environment to the handle for this object, then loads and executes the script chunk.
2. During loading, `init(data)` is available inside the script environment and receives the object's JSON entry for that `"lua:..."` key as a Lua table. Its `env["init"]` is called from `DeSerialize` if defined.
3. The following script functions are captured if defined: `start`, `update`, `onDestroy`, `onCollisionEnter`, `onCollisionExit`, `onTriggerEnter`, `onTriggerExit`.
4. **First `Update` call**: `LuaComponent` calls the script's `start()` exactly once (guarded by `started`), then calls `update(dt)`.
5. **Subsequent frames**: only `update(dt)` runs.
6. **On destruction**: if `onDestroy` is defined, it runs.

### Callback arguments

| Lua function | Arguments |
|--------------|-----------|
| `start()` | none |
| `update(dt)` | `dt` — seconds since previous frame (float) |
| `onDestroy()` | none |
| `onCollisionEnter(other)` | `other` — `GameObject` handle |
| `onCollisionExit(other)` | `other` — `GameObject` handle |
| `onTriggerEnter(other)` | `other` — `GameObject` handle |
| `onTriggerExit(other)` | `other` — `GameObject` handle |
| `init(data)` | `data` — the JSON value under the `"lua:<path>"` key, converted to a Lua table |

### Environment and `owner`

Each `LuaComponent` gets its own `sol::environment` whose parent is the global Lua table. Scripts therefore see all engine globals (`Log`, `Input`, `Objects`, and so on) plus one extra value: `owner`, a `GameObject` handle for the object the script is attached to.

## Error handling

Errors from Lua are caught per call and logged; they do not terminate the engine. Specifically:

- A chunk that fails to load or execute during `awake()` prevents the component's script functions from being registered. The error is logged with the script path.
- Errors in `init`, `start`, `update`, `onDestroy`, and collision/trigger callbacks are caught and logged with the script path.
- Errors in per-object collision callbacks set directly on a `GameObject` are caught and logged.

## Note on `core.d.lua`

The `init()` function declared in the IDE autocomplete file is **not** invoked by the engine. It exists only in `core.d.lua`. The actual engine entry points are `main.cpp` and `run()`. The real per-script hooks are `awake` (implicit, when the script loads), `init`, `start`, `update`, and the collision/trigger callbacks.