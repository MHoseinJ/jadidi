# Mouse

This page is a hub for mouse input.

## What you need to know

Mouse button constants live in the `Mouse` table (`LEFT`, `RIGHT`, `MIDDLE`, `X1`, `X2`). Input queries are on the `Input` global (`mousePressed`, `mouseDown`, `mouseUp`). Cursor position is available as a `Vector2` in two forms: screen space (`Mouse.position()`) and world space (`Mouse.world_position()`, which accounts for the camera).

## Where each part lives

| What | Where |
|------|-------|
| `Mouse.LEFT`, `Mouse.RIGHT`, `Mouse.MIDDLE`, `Mouse.X1`, `Mouse.X2` | [Engine Overview → Mouse](engine.md#mouse) |
| `Mouse.position()` and `Mouse.world_position()` | [Engine Overview → Mouse](engine.md#mouse) |
| `Input.mousePressed`, `mouseDown`, `mouseUp` | [Engine Overview → Input](engine.md#input) |
| The `Camera` type used by `world_position()` | [Objects → Camera](objects.md#camera) |
| The `Button` component for clickable UI | [Components → Button](components.md#button) |

## Typical usage

    -- Click in world space (for example, spawn a bullet at the cursor)
    if Input.mousePressed(Mouse.LEFT) then
        local p = Mouse.world_position()
        local bullet = Objects.create("Bullet")
        bullet.transform.position = p
    end

    -- Hover over something (using BoxCollider:overlap with a point)
    local p = Mouse.world_position()
    local box = someObject:getComponent("boxCollider")
    if box and box:overlap(p) then
        -- cursor is over the box
    end

For clickable UI, prefer the `Button` component, which handles the input-to-callback wiring. Use `Input.mousePressed` for world-space interactions.