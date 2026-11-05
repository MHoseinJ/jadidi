# Physics

This page is a hub for physics.

## What you need to know

Jadidi's physics is 2D, with axis-aligned boxes as the only collider shape. A body is created either by a `BoxCollider` on its own (static body) or by a `Rigidbody` (which takes over the collider). Bodies come in three kinds: static, kinematic, and dynamic. Collisions and triggers are delivered to components and to Lua callbacks.

The physics world is created per scene, from the scene file's `physics.gravity`. Changing scenes recreates the world.

## Where each part lives

| What | Where |
|------|-------|
| The `Physics` global: `raycast`, `setGravity` | [Engine Overview → Physics](engine.md#physics) |
| The `RaycastHit` result type | [Engine Overview → Physics](engine.md#physics) |
| The `BoxCollider` component | [Components → BoxCollider](components.md#boxcollider) |
| The `Rigidbody` component | [Components → Rigidbody](components.md#rigidbody) |
| Collision and trigger callbacks on a game object | [Objects → GameObject handle](objects.md#gameobject-handle) |
| Where physics events are delivered in the frame | [Lifecycle → Main loop](lifecycle.md#2-run--main-loop) |
| Scene-level gravity (`physics.gravity`) | [Formats → Scene files](../formats/scene.md#top-level-shape) |
| The dynamic-rigidbody-on-child crash and other constraints | [Known Limitations](../limitations.md#critical-dynamic-rigidbodies-on-child-objects-crash-the-engine) |

## Typical setup

1. Set gravity in the scene file (`physics.gravity`), or call `Physics.setGravity` from Lua.
2. Give objects a `boxCollider` (with `x`/`y` for size — see [the JSON field naming caveat](../limitations.md#json-field-names-boxcollider-uses-x--y-not-w--h)).
3. For moving objects, add a `rigidbody` with `"bodyType": "dynamic"`. Place the object at the scene root, not as a child of another object.
4. React to collisions with `obj.onCollisionEnter = function(other) ... end`, or define `onCollisionEnter(other)` inside the object's Lua script.

`Rigidbody::Update` writes the body's position back into the object's transform every frame. Scripts should not write `owner.transform.position` on a dynamic rigidbody and expect it to stick.