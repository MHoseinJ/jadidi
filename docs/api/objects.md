# Objects, Transform, and Camera

## GameObject handle

In Lua, game objects are represented by a `GameObject` handle (`GameObjectHandle` in C++). A handle stores the object's ID; accessors resolve the object each time they are used. If the object has been destroyed, accessors return `nil` and `valid` is `false`.

Handles are returned by `Objects.find`, `Objects.create`, and the collision/trigger callbacks. `tostring(handle)` yields `GameObject(id=<id>, valid=<true|false>)`.

### Members

| Member | Kind | Description |
|--------|------|-------------|
| `valid` | property (bool) | Whether the object still exists |
| `isValid()` | method (bool) | Whether the object still exists |
| `id` | property (number, read) | Numeric ID of the object |
| `name` | property (string) | Object name; assignable |
| `tag` | property (string) | Object tag; assignable |
| `transform` | property (`Transform`) | The object's transform; assignable (copies the transform) |
| `onCollisionEnter` | property (function or `nil`) | Callback: `f(other)` where `other` is a handle |
| `onCollisionExit` | property (function or `nil`) | Callback: `f(other)` |
| `onTriggerEnter` | property (function or `nil`) | Callback: `f(other)` |
| `onTriggerExit` | property (function or `nil`) | Callback: `f(other)` |
| `addComponent(name)` | method | Adds a component by factory name; returns the component, or `nil` on failure |
| `getComponent(name)` | method | Returns the component with that name, or `nil` |
| `destroy()` | method | Destroys the object |

### Equality

`a == b` is true when both handles refer to the same object ID. If either object has been destroyed, the handle is still comparable by ID — but no accessors on it will succeed.

### Notes

- `onCollision*` and `onTrigger*` callbacks set directly on a handle are invoked **in addition to** any `LuaComponent` callbacks (both run for the same event, from `Engine.cpp`).
- `addComponent` refuses `"transform"` (a `GameObject` always has exactly one `Transform`); it logs an error and returns `nil`.
- `addComponent("lua:<path>")` attaches a `LuaComponent` and runs the script's `awake()`, then `init(data)` with an empty table. This is the dynamic equivalent of adding a `"lua:<path>"` key in a scene file.

## Transform

Every `GameObject` has one `Transform`, exposed as `obj.transform`. It stores local position, local scale, and local rotation, and provides world-space accessors.

### Properties

| Property | Type | Access | Description |
|----------|------|--------|-------------|
| `position` | `Vector2` | read/write | Local position (assigning calls `setLocalPosition`) |
| `scale` | `Vector2` | read/write | Local scale (assigning calls `setLocalScale`) |
| `rotation` | number | read/write | Local rotation in degrees (assigning calls `setLocalRotation`) |
| `worldPosition` | `Vector2` | read-only | World position |
| `worldScale` | `Vector2` | read-only | World scale |
| `worldRotation` | number | read-only | World rotation in degrees |

Assigning `obj.transform = other` (via the `GameObject` handle) copies the whole transform.

### Notes

- Angles are in degrees. The engine does not wrap rotations.
- Changing a parent's transform marks children dirty; world accessors recompute lazily.

## Camera

The active camera is bound as a global named `Camera`. It is not a table with functions — it is a user type instance pointing at the engine's active camera.

### Fields

| Field | Type | Description |
|-------|------|-------------|
| `transform` | `Transform` | The camera's transform |
| `zoom` | number | Camera zoom factor |

### Notes

- `Camera` refers to the C++ global `camera` (`GameObject.h`), which is what the renderer uses.
- There is also a `Scene::camera` field in C++, but it is not exposed to Lua. Use the global `Camera` when reading or writing camera state from scripts.