# Input

This page is a hub for keyboard and mouse input.

## What you need to know

Input is queried per frame from Lua through the `Input` global. Key constants live in the `Key` table, mouse button constants in the `Mouse` table, and the mouse position is available in both screen space and world space.

Every query answers a different question:

- `keyPressed` / `mousePressed` — true only on the frame the key or button went down.
- `keyDown` / `mouseDown` — true every frame while the key or button is held.
- `keyUp` / `mouseUp` — true only on the frame the key or button was released.

For movement-style input, use the `*Down` variants. For a one-shot action (fire, jump, open menu), use the `*Pressed` variant.

## Where each part lives

| What | Where |
|------|-------|
| `Input.keyPressed`, `keyDown`, `keyUp`, `mousePressed`, `mouseDown`, `mouseUp` | [Engine Overview → Input](engine.md#input) |
| The `Key` table and its naming rules | [Engine Overview → Key](engine.md#key) |
| The `Mouse` table: button constants, `position()`, `world_position()` | [Engine Overview → Mouse](engine.md#mouse) |
| When input is updated in the frame | [Lifecycle → Main loop](lifecycle.md#2-run--main-loop) |

## Typical patterns

- Held movement: `if Input.keyDown(Key.LEFT) then ... end`
- Edge-triggered action: `if Input.keyPressed(Key.SPACE) then ... end`
- Aim at cursor: `local p = Mouse.world_position()`
- Click in world space: `if Input.mousePressed(Mouse.LEFT) then ... end`

See [Engine Overview → Input](engine.md#input) for the full API.