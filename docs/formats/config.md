# config.json

The engine reads `config.json` from the project directory at startup (`Engine::init`). If the file is missing or unreadable, the engine writes a default one and continues.

A complete example:

    {
        "window": {
            "fullscreen": false,
            "height": 720,
            "icon": "icon.bmp",
            "renderer": "opengl",
            "title": "MyGame",
            "width": 1280
        }
    }

## Fields

All fields live under `window`.

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| `title` | string | `"jadidi"` | Window title |
| `width` | integer | `1280` | Window width in pixels (ignored when `fullscreen` is true) |
| `height` | integer | `720` | Window height in pixels (ignored when `fullscreen` is true) |
| `fullscreen` | bool | `true` when writing the default file; the parsed default is also `true` | Requests desktop fullscreen. When true, the display bounds are used and `width`/`height` are replaced |
| `icon` | string | `"icon.bmp"` | Path to a BMP file used as the window icon. Failure to load is logged as a warning and is not fatal |
| `renderer` | string | `"opengl"` | Renderer backend. `"opengl"` creates a GL 3.3 core-profile context and uses the OpenGL renderer/texture backend. `"sdl"` uses the SDL renderer/texture backend. Any other value logs a warning and is treated as `"opengl"` |

## Behavior

- If `config.json` is missing or `cfg.load()` fails, the engine writes the default object above and uses it. The default `fullscreen` in the written file is `false`.
- The parsed default for `fullscreen` is `true` (`windowCfg.value("fullscreen", true)`), so a file that omits `fullscreen` starts fullscreen.
- `renderer` is validated against `"opengl"` and `"sdl"`. If invalid, a warning is logged, and the rest of the code path behaves as if `"opengl"` was selected, because the comparison `rendererBackend == "opengl"` fails and falls into the SDL branch — but the window flags still request an OpenGL context because the earlier check does not modify `rendererBackend`. This is the current behavior; treat `"opengl"` as the expected value unless you are testing the SDL backend deliberately.

## See also

- [Getting Started](../getting-started.md) — project layout and a minimal `config.json`
- [Scene files](scene.md)