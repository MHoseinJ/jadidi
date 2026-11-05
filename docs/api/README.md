# API Reference

This section documents the Lua-facing API of Jadidi. Every entry has been verified against the C++ bindings. If a name is not listed here, it is not exposed to Lua.

## Global namespaces

- [Engine Overview](engine.md) — `Log`, `State`, `Input`, `Key`, `Mouse`, `Json`, `Engine`, `Screen`, `Scene`, `Objects`, `Physics`, `AudioSystem`, `Camera`, `registerFunction`, `isValid`
- [Lifecycle](lifecycle.md) — process startup, main loop, scene loading, and per-script hooks

## Types

- [Vector2](math.md#vector2) — 2D vector with arithmetic operators
- [Color](math.md#color) — RGBA color
- [SDL_Rect](math.md#sdl_rect) — rectangle with `x`, `y`, `w`, `h`
- [Transform](objects.md#transform) — position, scale, rotation (local and world)
- [GameObject](objects.md#gameobject-handle) — handle to a game object
- [Camera](objects.md#camera) — active camera
- [Component](components.md#component-base) — base type shared by all components
- Components:
  - [Sprite](components.md#sprite)
  - [Animator](components.md#animator)
  - [Audio](components.md#audio)
  - [Text](components.md#text)
  - [Button](components.md#button)
  - [BoxCollider](components.md#boxcollider)
  - [Rigidbody](components.md#rigidbody)

## Component names

Components can be added from Lua via `obj:addComponent(name)` and looked up via `obj:getComponent(name)`. The following names are accepted by the factory:

| Name | Type |
|------|------|
| `sprite` | `Sprite` |
| `animator` | `Animator` |
| `text` | `Text` |
| `button` | `Button` |
| `audio` | `Audio` |
| `boxCollider` | `BoxCollider` |
| `rigidbody` | `Rigidbody` |
| `lua:<path>` | `LuaComponent` — attaches the Lua script at `<path>` |

`transform` also exists, but `addComponent("transform")` is rejected; a `GameObject` always has exactly one `Transform` accessible via `obj.transform`.

## See also

- [Formats](../formats/README.md) — JSON scene and config schemas
- [Examples](../examples/README.md) — complete projects