# Audio

Audio in Jadidi has two layers: loading sounds, and playing them. This page is a hub.

## What you need to know

Sound effects are loaded once through the `AudioSystem.load` global, then played by name through an `Audio` component on a game object. There is no general-purpose sound mixer, no music queue, and no separate `Audio` namespace beyond `AudioSystem`. Music is supported at the `AudioSystem` level, but the `Audio` component only plays sound effects.

## Where each part lives

| What | Where |
|------|-------|
| The `AudioSystem.load(name, path, isMusic)` global | [Engine Overview → AudioSystem](engine.md#audiosystem) |
| The `Audio` component: `Play`, `Stop`, `volume`, `loops`, `spatial`, `maxDistance` | [Components → Audio](components.md#audio) |
| How to declare an `Audio` component in a scene | [Formats → Scene files → Audio](../formats/scene.md#audio) |
| Known limitation: `Audio.channel` does not reflect the actual channel | [Known Limitations → Audio.channel](../limitations.md#audiochannel-does-not-reflect-the-actual-playback-channel) |

## Typical workflow

1. At startup (from a scene script's `start()` or an `init()`), call `AudioSystem.load("shoot", "Sounds/shoot.wav", false)`.
2. On the object that should play it, add an `Audio` component.
3. Call `audio:Play("shoot")`, or set `audio.name = "shoot"` and let the scene drive it.

Spatial audio is computed from the distance between the object's world position and the camera's world position, attenuated by `maxDistance` when `spatial` is true.

## See also

- [Lifecycle → Lua script lifecycle](lifecycle.md#lua-script-lifecycle-luacomponent) — where to load sounds