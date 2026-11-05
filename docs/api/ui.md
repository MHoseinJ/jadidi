# UI

This page is a hub for user-interface components.

## What you need to know

Jadidi's UI today is limited to the `Button` component, which draws an area and invokes Lua functions on mouse input. There is no general UI layout system, no labels beyond the `Text` component, no panels, no menus — those are built from `Sprite`, `Text`, and `Button` on game objects. All UI is in the same world as gameplay objects; there is no separate screen-space UI layer in this release.

`UIManager::Update()` runs once per frame (see the main loop). Button callbacks are dispatched internally.

## Where each part lives

| What | Where |
|------|-------|
| The `Button` component: `addFunction`, `zOrder` | [Components → Button](components.md#button) |
| Declaring a `Button` and its `functions` array in a scene file | [Formats → Scene files → Button](../formats/scene.md#button) |
| The `Text` component (for labels) | [Components → Text](components.md#text) |
| The `Sprite` component (for panels and icons) | [Components → Sprite](components.md#sprite) |
| `registerFunction(name, fn)` — how Button finds its callbacks | [Engine Overview → registerFunction](engine.md#registerfunction) |
| Where UI updates in the frame | [Lifecycle → Main loop](lifecycle.md#2-run--main-loop) |

## Typical setup

1. Create a `Sprite` for the visual (or a `Text` for a label).
2. Add a `Button` to the same object.
3. Register a function with `registerFunction("onStart", fn)`, or attach the function directly with `button:addFunction(fn, Mouse.LEFT)` from Lua.
4. If the button is defined in a scene file, list `{ "name": "...", "mouse": 1 }` entries under `button.functions`; the engine resolves each name through `registerFunction`.
5. Control draw order with `zOrder`.

UI objects are still game objects. They are affected by the camera and by `z_index` on sprites. For a screen-fixed UI, keep them at the camera's position or use a camera-follow pattern.