# Animation files

Animations live at `Animations/<name>.json`. They are loaded lazily: when an `Animator` plays an animation that is not already cached, it reads `Animations/<name>.json`.

## Shape

    {
        "name": "run",
        "frame_w": 32,
        "frame_h": 32,
        "start": 0,
        "end": 5,
        "frame_time": 0.1,
        "loop": true
    }

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| `name` | string | `""` | Animation name. When loaded through `Animator::Play(name)`, the name is set from the file path; the JSON field is read but the value from `Play` is authoritative |
| `frame_w` | integer | `0` | Width of one frame in the sprite sheet |
| `frame_h` | integer | `0` | Height of one frame |
| `start` | integer | `0` | First frame index |
| `end` | integer | `start` | Last frame index (inclusive) |
| `frame_time` | number | `0.1` | Seconds per frame |
| `loop` | bool | `false` | Whether the animation loops |

## Behavior

- The sprite sheet is the `Sprite` sibling's texture. Frame `n` uses `srcRect = { x: n * frame_w, y: 0, w: frame_w, h: frame_h }`.
- `Animator` accumulates `dt * speed`. Each time the accumulator reaches `frame_time`, `current_frame` advances by one.
- When `current_frame > end`:
  - If `loop` is true, wrap to `start`.
  - Otherwise, clamp to `end` and set `playing = false`.
- `start`/`end` are frame indices, not pixel coordinates.
- Only the first row of the sprite sheet is used; the engine sets `srcRect.y = 0` unconditionally.

## Missing or invalid files

If `Animations/<name>.json` is missing or invalid, `Play` logs `Invalid animation json: <name>` and does not change the current animation.

## Registering animations from code

The `Animation` struct is not exposed to Lua. Animations must be loaded from JSON. To register an animation without a file, that is a C++ change (`Animator::Register`); there is no Lua equivalent at this time.

## See also

- [Animator component](../api/components.md#animator)
- [Scene files](scene.md)