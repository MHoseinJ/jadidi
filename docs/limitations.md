# Known Limitations and Engine Quirks

This page lists behaviors that are either bugs, surprising, or constraints you need to design around. Everything here was verified against the C++ source of the current release. Each item says what happens, why, and what to do about it.

This page is expected to change between releases. When a limitation is fixed, its entry should be removed or moved to a "Fixed in" note.

## Critical: dynamic rigidbodies on child objects crash the engine

**What happens.** If an object is a child of another object (listed under a parent's `children` in a scene file) and has a `rigidbody` with `"bodyType": "dynamic"`, the engine logs to stderr:

    Rigidbody cannot be added to a child GameObject: <name>

It does not create a physics body. But the `Rigidbody` component is still attached and `Rigidbody::Update` runs every frame, calling into Box2D with an invalid body handle. On the first frame, Box2D's assertion fires:

    BOX2D ASSERTION: b2Body_IsValid( bodyId ), ...

and the process terminates with `SIGILL`.

**Why.** `Rigidbody::OnCreate` checks `bodyType == Dynamic && owner->getParent()` and returns early without setting up the body. `Rigidbody::Update` and the property setters do not check whether the body was created.

**What to do.** Never put a dynamic `Rigidbody` on a child object, whether via JSON or via `obj:addComponent("rigidbody")` + `rb.bodyType = 2`. If you need a dynamic object, place it at the top level of the scene's `objects` array. `Static` and `Kinematic` bodies are allowed on children.

**Not fixed in this release.** A guard in `Rigidbody::Update` and the setters (`if (!object.isValid()) return;`) would turn the crash into a silent no-op. That change is not in the current build.

## JSON field names: `BoxCollider` uses `x` / `y`, not `w` / `h`

**What happens.** In a scene file, `BoxCollider` size is written as:

    "boxCollider": { "x": 32, "y": 32, "isTrigger": false }

where `x` is the width and `y` is the height. This differs from `SDL_Rect`'s convention (`w` / `h`) used elsewhere, such as `sprite.src`.

**Why.** `BoxCollider::DeSerialize` reads `"x"` and `"y"` into `size.x` and `size.y`. `BoxCollider::Serialize` writes the same names. This is the current schema.

**What to do.** Use `x` and `y` when writing a collider in JSON. Do not use `w` and `h` — they are ignored and the collider gets zero size, which means it will not register with the physics engine.

## Component initialization order within an object is not the JSON order

**What happens.** When a scene object lists several components, they are not added in the order they appear in the JSON file. The factory iterates keys from an internal `std::unordered_map`, so the order is unspecified. Example: a `Hand` object with `boxCollider`, `sprite`, `rigidbody` in that JSON order may be initialized as `boxCollider`, `rigidbody`, `sprite`.

**Why.** Two-pass loading (Lua components first, then others) inside a `std::unordered_map`. Map iteration order is unspecified by the C++ standard and may vary between builds.

**What to do.** Two components have sibling dependencies:

- `Animator::OnCreate` looks up a sibling `Sprite`. If the sprite is not yet present, `sprite` is `nullptr`; `Animator::Update` retries the lookup on the next frame, so it recovers, but a `Sprite` is genuinely required for animation to do anything.
- `Rigidbody::OnCreate` looks up a sibling `BoxCollider`. If the collider is not yet present, the body is created with `hasCollider = false` and zero size, and the collider does not retroactively register itself. **This is not recovered.**

If order matters for an object, add the components from Lua with `obj:addComponent(...)` in the order you want. Lua `addComponent` runs immediately, in call order, so the order is deterministic.

For a `Rigidbody`, make sure its `BoxCollider` is added first via Lua if you are constructing the object dynamically. For scene files, this is a real hazard: the collider and rigidbody may initialize in either order.

## `Audio.channel` does not reflect the actual playback channel

**What happens.** After `audio:Play(name)`, the `channel` property is not updated to the channel Box2D (well, SDL_mixer) actually chose. Reading `audio.channel` gives the deserialized value from the scene file, not the live one.

**Why.** In `Audio::Play`:

    if (const auto channel_played_on = AudioSystem::getInstance().PlaySound(audio_name, -1, loops);
        channel_played_on < 0) {
        gameLog("failed to play sound: " + audio_name, ERROR);
        channel = channel_played_on;
    }

`channel` is assigned only on failure. On success it is left as-is.

**What to do.** Do not rely on `audio.channel` after calling `Play`. If you need the actual channel, that information is currently not exposed to Lua. Use distinct `Audio` components (one per sound you want to control independently) instead.

**Not fixed in this release.**

## `Sprite.size` and `Text.size` are methods, not properties

**What happens.** `sprite.size` returns a function, not a `Vector2`. You need `sprite:size()` to get the value.

**Why.** `BindECS.cpp` binds `"size", &Sprite::size` as a bare member function pointer, not as a `sol::property`. sol2 exposes zero-argument member functions as callable members, not properties. The same applies to `Text`.

**What to do.** Call `sprite:size()` and `text:size()`. For `BoxCollider`, `size` **is** a property (`&BoxCollider::size` is a member variable), and it is also exposed on the generic `Component` type as a property with a setter that rebuilds the body.

Note: assigning `collider.size = v` through the `BoxCollider` user type sets the field directly and does **not** call `rebuildBody`. Assigning through the generic `Component` setter (`comp.size = v`) does call `rebuildBody`. Both are Lua writes; the behavior differs by which user type you hold. This is inconsistent and should probably be fixed; see the source in `BindECS.cpp` under the `Component` and `BoxCollider` bindings.

## `config.json` renderer validation is soft

**What happens.** If `renderer` is not `"opengl"` or `"sdl"`, the engine logs a warning and does not change the value. Later, the window flags are chosen by checking `rendererBackend == "opengl"`, which fails for an invalid value, so the window gets SDL flags rather than OpenGL flags. But the renderer backend selection later does the same check and falls into the SDL branch. The net result is an SDL renderer with no GL context, despite the warning suggesting a fallback to `"opengl"`.

**Why.** The warning says "Defaulting to 'opengl'" but the code does not set `rendererBackend = "opengl"`; it just continues.

**What to do.** Use only `"opengl"` or `"sdl"`. An invalid value will run the SDL backend with a warning, which may be fine, but the warning text is misleading. If you intended OpenGL, fix the value.

## `core.d.lua` declares an `init()` hook that the engine never calls

**What happens.** `core.d.lua` declares a global `init()` function and a `start()` / `update(dt)` pair as if they were engine hooks. Only `start()` and `update(dt)` are real — they are the `LuaComponent` script hooks. The `init()` in `core.d.lua` is not invoked by the engine.

**Why.** The file is IDE autocompletion metadata. It is maintained separately from the C++ and is not authoritative.

**What to do.** Do not rely on a global `init()` being called. The per-script hook that receives the `lua:<path>` data is the script's own `init(data)`, declared inside the script file itself, not in `core.d.lua`. See [Lifecycle](api/lifecycle.md#lua-script-lifecycle-luacomponent).

## `Json` binding is not wired in the version inspected

**What happens.** `LuaBindings.h` declares `bindJson`, and `BindJson.cpp` defines it, but `Lua::init()` in `LuaBindings.cpp` does not call it. If that is still the case in your build, the `Json` table does not exist at runtime.

**Why.** `Lua::init()` calls, in order: `bindCore`, `bindMath`, `bindInput`, `bindDebug`, `bindAsset`, `bindState`, `bindECS`, `bindScene`. There is no `bindJson` call.

**Status.** The engine author reports this as fixed in their current working copy. If your build includes the fix, ignore this entry; if you are on a build before the fix, `Json` will be `nil` and any script calling `Json.read` or `Json.write` will error.

**What to do.** Confirm the behavior on the build you are using. This entry should be removed once the fix is verified in a released build.

## `BindAsset` is a TODO

**What happens.** `BindAsset.cpp` contains:

    void LuaBindings::bindAsset(sol::state& lua) {
        // TODO: bind asset system
    }

There is no Lua asset API in this release. Assets are loaded by components (`Sprite` from a texture path, `Text` through `FontManager`, `Audio` through `AudioSystem.load`), not through a general-purpose asset namespace.

**What to do.** Load assets through the component that uses them. Do not look for a general `Asset` namespace in Lua.

## `Rigidbody.setPosition` is not exposed to Lua

**What happens.** `Rigidbody` has a `setPosition` method in C++, but the Lua user type binds `setVelocity`, `setDensity`, `setFriction`, `setBodyType`, and `applyImpulse` — not `setPosition`. To teleport a rigidbody from Lua, use `owner.transform.position = ...`, which writes the transform. However, `Rigidbody::Update` then writes the transform back from the physics body, so a transform write alone will be overwritten next frame.

**Why.** `Rigidbody::Update` does:

    transform->setWorldPosition(physics->getPosition(&object));

**What to do.** Teleporting a rigidbody from Lua is not currently supported cleanly. The available paths are:

- Use `applyImpulse` or set `velocity` to move the body under physics.
- For static or kinematic bodies, modify the transform (it will still be overwritten by `Update`, but if the body is not moving it converges).

This is a gap, not a bug. It may be addressed in a future release.

## `Scene.load` is deferred to end-of-frame

**What happens.** Calling `Scene.load(name)` from Lua does not load the scene immediately. It records a pending scene name, and the engine processes it at the end of the current frame, after rendering and after all object updates.

**Why.** `SceneManager::requestSwitchScene` stores `pendingScene`, and `Engine::run` calls `SceneManager::processPendingScene()` after `endFrame`.

**What to do.** Do not assume the scene has changed by the next line in your script. If you need to react to the switch, do it in `start()` of a script in the new scene.

## `Engine.exit()` completes the current frame

**What happens.** `Engine.exit()` calls `Input::Quit()`. The main loop checks `Input::QuitRequested()` at the top of each iteration, so the current frame finishes and the loop exits on the next iteration.

**Why.** This is intentional: it lets the frame complete cleanly (rendering, deferred scene processing) before shutdown.

**What to do.** If you need the process to stop after a specific line, do not depend on an immediate exit. The current frame completes, then the engine shuts down.

## `Camera` is a user type instance, not a table

**What happens.** `Camera` is bound as `lua["Camera"] = &camera`, a `Camera` user type. Accessing it is fine (`Camera.zoom`), but it is not a namespace with methods. `Camera` and `Scene::camera` (in C++) are different objects; only `Camera` is exposed to Lua.

**What to do.** Read and write camera state through `Camera.transform` and `Camera.zoom`. There is no `Camera.lookAt`, `Camera.follow`, or similar. Camera movement must be done by assigning the transform from a script.

## `State` stores `sol::object` references

**What happens.** `State` stores Lua objects in a C++ `std::unordered_map<std::string, sol::object>`. Values are copied into the map; tables are stored by reference to the Lua object.

**Why.** `State::set` takes `const sol::object&` and copies it into the map.

**What to do.** This is generally fine, but if you store a table in `State` and then mutate it elsewhere, both references see the change. If you need snapshots, store primitive values or serialize with `Json.write`.

## `LuaComponent` is found by exact `typeName()`

**What happens.** `obj:getComponent("lua:Scripts/player.lua")` works only if the path matches exactly. There is no way to enumerate components from Lua, and `getComponent` by a shortened name will fail.

**What to do.** Keep track of the script path if you need to find the component from another script. Or store a reference to the script's data in `State` rather than looking up the component.

## `Scenes/` and `Animations/` paths are relative to the working directory

**What happens.** The engine opens `Scenes/<name>.json` and `Animations/<name>.json` relative to the process working directory, not relative to the executable.

**What to do.** Run the engine from the project directory (where `config.json` lives), as the docs describe. Running `./jadidi` from elsewhere will fail to find scenes.

## Reporting a limitation

If you find a behavior that should be on this list, include the engine version, the scene file, the script, and the engine log output. The most useful bug reports include a minimal reproduction: a single-object scene with the minimum components needed to trigger the behavior. If the engine crashes, the log on stderr is more useful than the in-game log; capture both.