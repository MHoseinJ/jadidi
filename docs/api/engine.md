# Engine Overview

This page lists the global namespaces and objects that Jadidi exposes to Lua. All entries are verified against the C++ bindings.

> Signatures shown are the **Lua-facing** signatures. Optional parameters are shown in brackets. If a function throws a Lua error for the wrong receiver type, that is noted.

## Global namespaces

| Name | Description |
|------|-------------|
| `Log` | Logging |
| `State` | Global key-value store shared across scripts |
| `Input` | Keyboard and mouse input queries |
| `Key` | Named key constants |
| `Mouse` | Mouse button constants and cursor position |
| `Json` | JSON read/write helpers |
| `Engine` | Engine control |
| `Screen` | Screen information |
| `Scene` | Scene loading |
| `Objects` | Object lookup, creation, and deletion |
| `Physics` | Global physics queries and settings |
| `AudioSystem` | Sound loading |
| `Camera` | The active camera |
| `registerFunction` | Registers a named global function |
| `isValid` | Checks whether an object handle is valid |

## Log

| Function | Signature | Description |
|----------|-----------|-------------|
| `clear` | `Log.clear()` | Clears the in-game log |
| `print` | `Log.print(msg)` | Prints a message |
| `info` | `Log.info(msg)` | Logs an informational message |
| `warn` | `Log.warn(msg)` | Logs a warning |
| `error` | `Log.error(msg)` | Logs an error |

All logging functions accept a string.

## State

An in-memory key-value store. Keys are strings; values are any Lua value (stored as a `sol::object`).

| Function | Signature | Description |
|----------|-----------|-------------|
| `set` | `State.set(key, value)` | Stores `value` under `key` |
| `get` | `State.get(key)` | Returns the stored value, or `nil` if `key` is not set |
| `exists` | `State.exists(key)` | Returns `true` if `key` is set |
| `remove` | `State.remove(key)` | Removes `key` |
| `clear` | `State.clear()` | Removes all keys |

## Input

| Function | Signature | Description |
|----------|-----------|-------------|
| `keyPressed` | `Input.keyPressed(key)` | `true` on the frame the key went down |
| `keyDown` | `Input.keyDown(key)` | `true` while the key is held |
| `keyUp` | `Input.keyUp(key)` | `true` on the frame the key was released |
| `mousePressed` | `Input.mousePressed(button)` | `true` on the frame the button went down |
| `mouseDown` | `Input.mouseDown(button)` | `true` while the button is held |
| `mouseUp` | `Input.mouseUp(button)` | `true` on the frame the button was released |

`key` may be an integer scancode (usually taken from `Key.*`) or a key name string. `button` is an integer button constant from `Mouse.*`.

## Key

`Key` is a table mapping sanitized SDL scancode names to integer scancode values. Examples of entry names: `Key.A`, `Key.SPACE`, `Key.ESCAPE`, `Key.LEFT`. Non-alphanumeric characters in the SDL name are converted to `_`, and names that would clash with Lua keywords get a `_KEY` suffix. All entries are strings in Lua (`Key["A"]`, `Key["SPACE"]`, ...).

The table is generated at startup from the SDL scancode list, so the exact set of entries depends on the SDL version in use. It is intended to be used with the `Input` functions.

## Mouse

`Mouse` contains both button constants and cursor helpers.

| Entry | Description |
|-------|-------------|
| `Mouse.LEFT` | Left button constant |
| `Mouse.RIGHT` | Right button constant |
| `Mouse.MIDDLE` | Middle button constant |
| `Mouse.X1` | Extra button 1 |
| `Mouse.X2` | Extra button 2 |
| `Mouse.position()` | Returns mouse position as a `Vector2` (screen space) |
| `Mouse.world_position()` | Returns mouse position as a `Vector2` (world space, using the camera) |

## Json

| Function | Signature | Description |
|----------|-----------|-------------|
| `read` | `Json.read(path)` | Reads and parses a JSON file. Returns a Lua table, or `nil` if the file does not exist. |
| `write` | `Json.write(path, table)` | Serializes a Lua table to JSON and writes it to `path`. Returns `true` on success. If `table` is not a Lua table, returns `false`. |

The conversion is bidirectional and follows the rules in the `LuaJSON` converter:

- JSON objects become Lua tables with string keys; JSON objects whose keys are all numeric strings become Lua tables with integer keys.
- JSON arrays become 1-based Lua arrays.
- JSON numbers become Lua numbers, JSON strings become Lua strings, booleans become booleans, and `null` becomes `nil`.
- When writing, a Lua table with only positive integer keys (starting at 1, no gaps) is written as a JSON array; any other table is written as a JSON object.

## Engine

| Function | Signature | Description |
|----------|-----------|-------------|
| `exit` | `Engine.exit()` | Requests engine shutdown. The current frame completes before the process exits. |

## Screen

| Function | Signature | Description |
|----------|-----------|-------------|
| `size` | `Screen.size()` | Returns a `Vector2` with the current window width and height |

## Scene

| Function | Signature | Description |
|----------|-----------|-------------|
| `load` | `Scene.load(name)` | Requests a scene switch. The switch is applied at the end of the current frame. `name` is the scene file name without the `.json` extension (for example, `"home"`). |

## Objects

The `Objects` table provides lookup, creation, and deletion of game objects. Objects are represented in Lua by a `GameObject` handle. Handles are valid only while the object exists; use `isValid` or `handle.valid` / `handle:isValid()` before relying on one.

| Function | Signature | Description |
|----------|-----------|-------------|
| `find` | `Objects.find(name)` | Returns a handle to the first object with that name, or `nil` |
| `find` | `Objects.find(id)` | Returns a handle to the object with that numeric ID, or `nil` |
| `create` | `Objects.create(name)` | Creates a new object with the given name and returns its handle |
| `deleteById` | `Objects.deleteById(id)` | Deletes the object with that ID |
| `deleteById` | `Objects.deleteById(handle)` | Deletes the object for that handle |
| `deleteByName` | `Objects.deleteByName(name)` | Deletes all objects with that name |
| `deleteByTag` | `Objects.deleteByTag(tag)` | Deletes all objects with that tag |

`isValid(obj)` is a global function: returns `true` if `obj` is a `GameObject` handle whose target still exists.

## GameObject handle

A handle refers to an existing object. Accessors on the handle are resolved each time they are read; if the object has been destroyed, they return `nil`.

| Member | Kind | Description |
|--------|------|-------------|
| `valid` | property (bool) | Whether the object still exists |
| `isValid()` | method (bool) | Whether the object still exists |
| `id` | property (number, read) | Numeric ID of the object |
| `name` | property (string) | Object name; assignable |
| `tag` | property (string) | Object tag; assignable |
| `transform` | property (`Transform`) | The object's transform; assignable (copies the transform) |
| `onCollisionEnter` | property (function or `nil`) | Callback receiving another object's handle |
| `onCollisionExit` | property (function or `nil`) | Callback receiving another object's handle |
| `onTriggerEnter` | property (function or `nil`) | Callback receiving another object's handle |
| `onTriggerExit` | property (function or `nil`) | Callback receiving another object's handle |
| `addComponent(name)` | method | Adds a component of the named type; returns the component or `nil` |
| `getComponent(name)` | method | Returns the component of the named type, or `nil` |
| `destroy()` | method | Destroys the object |

Handles compare by ID: `a == b` is true when they refer to the same object. `tostring(handle)` yields `GameObject(id=<id>, valid=<true|false>)`.

## Physics

| Function | Signature | Description |
|----------|-----------|-------------|
| `raycast` | `Physics.raycast(start, end)` | Casts a ray from `start` to `end` (both `Vector2`). Returns a `RaycastHit`. |
| `setGravity` | `Physics.setGravity(gravity)` | Sets the world gravity (a `Vector2`) |

`RaycastHit` is a userdata with read-only fields:

| Field | Type | Description |
|-------|------|-------------|
| `hit` | boolean | Whether the ray hit something |
| `point` | `Vector2` | Hit point |
| `normal` | `Vector2` | Hit normal |
| `fraction` | number | Fraction of the ray traveled before the hit |

If the physics world has not been created (no scene has been loaded yet), these functions throw a Lua error.

## AudioSystem

| Function | Signature | Description |
|----------|-----------|-------------|
| `load` | `AudioSystem.load(name, path, isMusic)` | Loads a sound under `name` from `path`. `isMusic` selects a streamed music track (for example, OGG) instead of a sound effect (WAV). |

Loaded sounds can be played through an `Audio` component by setting its `name`.

## Camera

`Camera` is a userdata referring to the engine's active camera.

| Field | Type | Description |
|-------|------|-------------|
| `transform` | `Transform` | Camera transform |
| `zoom` | number | Camera zoom factor |

## registerFunction

`registerFunction(name, func)` stores `func` under `name` for later retrieval by the engine. It is used by the engine to look up global Lua functions by name.

## Standard Lua libraries

`bindCore` opens these Lua standard libraries:

- `base`
- `math`
- `table`
- `string`

Other standard libraries are not opened by the engine.

## See also

- [Lifecycle](lifecycle.md) — when engine initialization, scenes, and script hooks run
- [API Reference](README.md) — components, transforms, and other user types