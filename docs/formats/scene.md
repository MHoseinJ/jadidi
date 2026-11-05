# Scene files

Scenes live at `Scenes/<name>.json`. The engine loads a scene by name; `run()` loads `"home"` at startup, which resolves to `Scenes/home.json`.

## Top-level shape

    {
        "physics": { "gravity": { "x": 0, "y": -9.8 } },
        "objects": [ ... ]
    }

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| `objects` | array | — | **Required.** Top-level object definitions |
| `physics` | object | — | Optional. When present, the engine reads `gravity` |
| `physics.gravity` | `Vector2` | `{0, -9.8}` | Gravity vector. Accepts `{ "x": ..., "y": ... }` or `[x, y]` |

If `objects` is missing, the loader logs an error and returns without loading the scene. If the file cannot be read or parsed, `Json::isValid()` is false and the loader logs `Failed to load scene: <name>` and returns.

## Object definition

Each entry in `objects` is an object. Children are nested in `children` and are recursively loaded with the same rules.

    {
        "name": "Player",
        "id": 7,
        "tag": "player",
        "transform": {
            "position": { "x": 0, "y": 0 },
            "rotation": 0,
            "scale": { "x": 1, "y": 1 }
        },
        "sprite": { "texture": "player.png" },
        "lua:Scripts/player.lua": {},
        "children": []
    }

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `name` | string | yes | Object name. Missing `name` logs an error and skips the object |
| `id` | integer | no | Object ID. If present, it is used (and `nextId` is advanced past it). If absent, an ID is assigned automatically |
| `tag` | string | no | Object tag |
| `transform` | object | no | Local transform. Omitted fields default to identity (`position {0,0}`, `scale {1,1}`, `rotation 0`) |
| `children` | array | no | Nested object definitions, loaded recursively with the same rules |

### Transform fields

| Field | Type | Default |
|-------|------|---------|
| `position` | `Vector2` | `{0, 0}` |
| `rotation` | number | `0` |
| `scale` | `Vector2` | `{1, 1}` |

## Components

Any key on the object that is not one of the reserved fields is treated as a component, except that keys beginning with `lua:` are handled first, in a separate pass.

Reserved keys (never treated as components): `name`, `tag`, `transform`, `id`, `children`.

### Two-pass component loading

For each object, the loader iterates keys twice:

1. **First pass — Lua components.** Keys matching `lua:*`. The factory creates a `LuaComponent`, sets its `scriptPath` to the text after `lua:`, calls `awake()` (which loads the script), then `DeSerialize` (which calls the script's `init(data)` with the key's value as a Lua table). Finally `addComponent` is called, which fires `OnCreate` (a no-op for `LuaComponent`).
2. **Second pass — regular components.** Keys not starting with `lua:` and not reserved. The factory creates the component by name; `DeSerialize` is called; `addComponent` is called.

If the factory cannot find a component name, it throws. The loader catches it at the scene level and logs, but does not abort the rest of the scene. Unknown keys that look like typos will be reported with a "Did you mean ...?" suggestion when the Levenshtein distance is at most 2.

### Initialization order within an object

Within each pass, component keys are processed in the order the factory returns them from an internal `std::unordered_map` — **not** the order they appear in the JSON. This means a component's `OnCreate` may run before another component on the same object that it depends on.

Two components have sibling dependencies:

- `Animator` looks for a `Sprite` on the same object during `OnCreate`. If `Sprite::OnCreate` has not run yet, `Animator::OnCreate` will set `sprite = nullptr`. `Animator::Update` retries the lookup on the first frame, so a missing-at-`OnCreate` sprite is recovered on the next `Update` — but if there is genuinely no `Sprite`, the animator stays inert.
- `Rigidbody` looks for a `BoxCollider` on the same object during `OnCreate`. If the collider is not yet present, the body is created with `hasCollider = false` and zero size. The collider does not retroactively register itself with the rigidbody. This is not recovered automatically.

If order matters for an object, add the components from Lua in the order you want, using `obj:addComponent(...)`, rather than relying on JSON key order. Lua `addComponent` adds a component immediately, in call order.

> For a component that needs a sibling `BoxCollider`, adding the collider first via Lua avoids the zero-size body issue.

### Component keys

The following keys are recognized by the factory:

| Key | Component | Schema |
|-----|-----------|--------|
| `sprite` | `Sprite` | [sprite](#sprite) |
| `animator` | `Animator` | [animator](#animator) |
| `text` | `Text` | [text](#text) |
| `button` | `Button` | [button](#button) |
| `audio` | `Audio` | [audio](#audio) |
| `boxCollider` | `BoxCollider` | [boxCollider](#boxcollider) |
| `rigidbody` | `Rigidbody` | [rigidbody](#rigidbody) |
| `lua:<path>` | `LuaComponent` | value is passed to the script's `init(data)` |

`transform` is reserved; `addComponent("transform")` is rejected at runtime.

### Sprite

    "sprite": {
        "texture": "player.png",
        "z_index": 0,
        "color": { "r": 255, "g": 255, "b": 255, "a": 255 },
        "src": { "x": 0, "y": 0, "w": 64, "h": 64 }
    }

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| `texture` | string | `""` | Texture path. Empty means no texture |
| `z_index` | integer | `0` | Draw order |
| `color` | `Color` | `{r:255, g:255, b:255, a:255}` | Tint |
| `src` | `SDL_Rect` | `{0, 0, 0, 0}` | Source rectangle. If `w`/`h` are 0, `OnCreate` uses the texture's size, or `16×16` when there is no texture |

### Animator

    "animator": {
        "speed": 1.0,
        "playing": true,
        "play": "idle"
    }

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| `speed` | number | `1.0` | Playback speed. Negative values are clamped to `0` at runtime |
| `playing` | bool | `true` | Whether playback starts enabled |
| `play` | string | `""` | Animation name to start on first `Update`. Empty means no auto-start |

### Text

    "text": {
        "text": "Hello",
        "size": 16,
        "font": "font",
        "color": { "r": 0, "g": 0, "b": 0, "a": 255 }
    }

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| `text` | string | `""` | Text content |
| `size` | integer | `16` | Font size in pixels |
| `font` | string | `"font"` | Font name loaded by `FontManager` |
| `color` | `Color` | `{r:0, g:0, b:0, a:255}` | Text color |

### Button

    "button": {
        "zOrder": 0,
        "functions": [
            { "name": "onStartClicked", "mouse": 1 }
        ]
    }

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| `zOrder` | integer | `0` | Button ordering |
| `functions` | array | `[]` | Registered functions |

Each entry in `functions`:

| Field | Type | Description |
|-------|------|-------------|
| `name` | string | Name of a function previously registered via `registerFunction(name, fn)` |
| `mouse` | integer | Mouse button constant (for example, `1` for left) |

If a name is empty or the function is not found, an error is logged and that entry is skipped.

### Audio

    "audio": {
        "name": "shoot",
        "loops": 1,
        "volume": 128,
        "spatial": false,
        "maxDistance": 5000,
        "channel": -1
    }

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| `name` | string | `""` | Sound name, previously loaded via `AudioSystem.load` |
| `loops` | integer | `1` | Loop count |
| `volume` | integer | `128` | Volume |
| `spatial` | bool | `false` | Enables spatial attenuation |
| `maxDistance` | number | `5000` | Maximum audible distance |
| `channel` | integer | `-1` | Channel index (`-1` = auto) |

### BoxCollider

    "boxCollider": {
        "x": 32,
        "y": 32,
        "isTrigger": false
    }

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| `x` | number | `0` | Width |
| `y` | number | `0` | Height |
| `isTrigger` | bool | `false` | Trigger flag |

Note: the field names are `x` and `y`, not `width`/`height` — this matches `BoxCollider::DeSerialize`.

### Rigidbody

    "rigidbody": {
        "bodyType": "dynamic",
        "density": 1.0,
        "friction": 1.0,
        "velocity": { "x": 0, "y": 0 }
    }

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| `bodyType` | string | `"static"` | `"static"`, `"kinematic"`, or `"dynamic"` |
| `density` | number | `1.0` | Body density |
| `friction` | number | `1.0` | Body friction |
| `velocity` | `Vector2` | `{0, 0}` | Initial velocity |

**Constraint — dynamic rigidbodies cannot be on child objects.** If an object has a `parent` (it is listed under another object's `children`) and its `rigidbody.bodyType` is `"dynamic"`, `Rigidbody::OnCreate` logs `Rigidbody cannot be added to a child GameObject: <name>` to `std::cerr` and returns without creating a physics body. The `Rigidbody` component is still attached to the object.

This is not a soft failure. `Rigidbody::Update` runs every frame and calls into the physics engine with the (invalid) body handle. The first frame after loading a scene that violates this rule will abort the process with a Box2D assertion (`b2Body_IsValid`), typically `SIGILL`. The engine will not continue with a broken body.

`"static"` and `"kinematic"` bodies are allowed on children. To use a dynamic body, place the object at the top level of `objects`, not under another object's `children`.

## Lua scripts

A `lua:` key attaches a `LuaComponent`. Its value is passed to the script's optional `init(data)` as a Lua table.

    "lua:Scripts/player.lua": {
        "speed": 100
    }

In `Scripts/player.lua`:

    function init(data)
        Log.print("speed = " .. tostring(data.speed))
    end

The script is loaded via `awake()` before any other component on the object is processed. This means an `init` function can use `owner:getComponent(...)` to inspect other components only after they are added — they are not yet present during `init`. If you need to read sibling components, do so from `start()`.

## Loading order

1. `onExit()` on the current scene.
2. Clear current objects and ID lookup.
3. Reset the physics world.
4. Read `Scenes/<name>.json`. If it cannot be read/parsed, log and return.
5. Read `physics.gravity` (default `{0, -9.8}`) and create the physics world.
6. Read the `objects` array. Missing `objects` logs an error and returns.
7. For each root object, recursively load it and its `children`.
8. `onEnter()` on the new scene.

## See also

- [Scene loading in the lifecycle](../api/lifecycle.md#scene-lifecycle)
- [Animation files](animation.md)