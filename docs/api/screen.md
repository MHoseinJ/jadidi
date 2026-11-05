# Screen

This page is a hub for screen and window information.

## What you need to know

The `Screen` global provides the current window size. There is no separate screen module — window size, fullscreen state, and renderer backend are configured in `config.json` at startup and are not changeable from Lua in this release.

## Where each part lives

| What | Where |
|------|-------|
| `Screen.size()` | [Engine Overview → Screen](engine.md#screen) |
| Window and renderer configuration (`config.json`) | [Formats → config.json](../formats/config.md) |
| The window size used for screen-space mouse coordinates | [Engine Overview → Mouse](engine.md#mouse) |

## Typical usage

    local size = Screen.size()
    Log.print("Window is " .. size.x .. "x" .. size.y)

    -- Center an object
    obj.transform.position = Vector2(size.x / 2, size.y / 2)

`Screen.size()` returns the window size in pixels. For world-space positions, remember that the camera transform and zoom affect the mapping from world to screen; see [Objects → Camera](objects.md#camera).