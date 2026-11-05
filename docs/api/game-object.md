# Game Objects

This page is a hub for everything about game objects: the Lua handle, creating and destroying objects, the transform, and how scripts attach to objects.

## What you need to know

In Lua, a game object is represented by a handle (`GameObjectHandle` in C++). The handle stores the object's ID, and accessors resolve the object each time they are read. Handles are returned by `Objects.find`, `Objects.create`, the collision callbacks, and `owner` inside a script attached to the object. A handle becomes invalid when the object is destroyed; `valid` and `isValid()` tell you which.

Every object has exactly one `Transform`. Components are attached to the object and run every frame through the object's `Update`.

## Where each part lives

| What | Where |
|------|-------|
| The `GameObject` handle: `valid`, `id`, `name`, `tag`, `transform`, collision callbacks, `addComponent`, `getComponent`, `destroy`, equality, `tostring` | [Objects → GameObject handle](objects.md#gameobject-handle) |
| The `Objects` global: `find`, `create`, `deleteById`, `deleteByName`, `deleteByTag` | [Engine Overview → Objects](engine.md#objects) |
| The `Transform` user type | [Objects → Transform](objects.md#transform) |
| The `Camera` global | [Objects → Camera](objects.md#camera) |
| Attaching a Lua script to an object (`"lua:<path>"`) | [Formats → Scene files → Lua scripts](../formats/scene.md#lua-scripts) |
| Script hooks (`init`, `start`, `update`, `onDestroy`, collision callbacks) | [Lifecycle → Lua script lifecycle](lifecycle.md#lua-script-lifecycle-luacomponent) |
| Object loading order during a scene load | [Formats → Scene files → Loading order](../formats/scene.md#loading-order) |

## Typical usage

- Find an object by name: `local player = Objects.find("Player")`
- Create an object at runtime: `local bullet = Objects.create("Bullet")`, then `bullet:addComponent("sprite")` and set `bullet.transform.position`.
- Destroy an object: `Objects.deleteByName("Bullet")`, or `obj:destroy()` from a handle.
- Check validity before using a handle from earlier: `if obj.valid then ... end` or `if isValid(obj) then ... end`.

See [Objects and Transform](objects.md) for the full handle and transform reference.