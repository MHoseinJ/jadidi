# Animation

Animation in Jadidi is split across three places. This page is a hub.

## What you need to know

An `Animator` component plays named animations on a sibling `Sprite`. The animation data (frame size, frame range, timing, looping) lives in a JSON file under `Animations/<name>.json`, loaded lazily the first time the animation is played.

## Where each part lives

| What | Where |
|------|-------|
| The `Animator` component: `Play`, `Pause`, `Resume`, `Stop`, `SetSpeed`, `speed`, `playing` | [Components → Animator](components.md#animator) |
| How animation frames are applied to the sprite | [Components → Animator → Behavior](components.md#animator) |
| The animation file schema (`frame_w`, `frame_h`, `start`, `end`, `frame_time`, `loop`) | [Formats → Animation files](../formats/animation.md) |
| How to declare an `Animator` in a scene | [Formats → Scene files → Animator](../formats/scene.md#animator) |
| The `Sprite` component that provides the texture | [Components → Sprite](components.md#sprite) |

## Typical setup

1. A sprite sheet is loaded through a `Sprite` component (`texture` field).
2. An `Animator` on the same object references animations by name.
3. Each animation is defined in `Animations/<name>.json`.
4. The animator updates the sprite's `srcRect` on each frame to select the right frame.

See [Formats → Animation files](../formats/animation.md) for the exact JSON, and [Components → Animator](components.md#animator) for the runtime API.