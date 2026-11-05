# Keyboard

This page is a hub for keyboard input.

## What you need to know

Keyboard input is queried through the `Input` global, using constants from the `Key` table. Keys can be queried by their integer scancode or, in many cases, by a name string. The `Key` table is generated at startup from SDL's scancode list, so the exact set of keys depends on the SDL version.

## Where each part lives

| What | Where |
|------|-------|
| `Input.keyPressed`, `keyDown`, `keyUp` | [Engine Overview → Input](engine.md#input) |
| The `Key` table, its naming, and Lua keyword handling | [Engine Overview → Key](engine.md#key) |
| When input is updated in the frame | [Lifecycle → Main loop](lifecycle.md#2-run--main-loop) |

## Typical usage

    -- Held movement
    if Input.keyDown(Key.LEFT)  then position.x = position.x - speed * dt end
    if Input.keyDown(Key.RIGHT) then position.x = position.x + speed * dt end

    -- Jump on press
    if Input.keyPressed(Key.SPACE) then jump() end

For a one-shot action, use `keyPressed`; for movement, use `keyDown`. `keyUp` is for "did the key come up this frame" checks, which are rarely what you want.

See [Engine Overview → Input](engine.md#input) and [Engine Overview → Key](engine.md#key).