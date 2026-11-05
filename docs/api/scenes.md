# Scenes (Lua)

This page is a hub for scene loading and switching **from Lua**. For the scene file format, see [Formats → Scene files](../formats/scene.md).

## What you need to know

A scene is loaded by name. `Scene.load(name)` requests a switch to `Scenes/<name>.json`. The switch is deferred to the end of the current frame; it does not happen synchronously. The engine loads `"home"` at startup, before any script runs.

Scene files themselves define objects, their transforms, their components, physics gravity, and Lua scripts. The Lua side only controls *which scene is loaded* and *when*.

## Where each part lives

| What | Where |
|------|-------|
| `Scene.load(name)` | [Engine Overview → Scene](engine.md#scene) |
| `Objects.find`, `Objects.create`, `Objects.deleteById`, etc. | [Engine Overview → Objects](engine.md#objects) |
| Scene file format (objects, transforms, components, physics) | [Formats → Scene files](../formats/scene.md) |
| Scene loading order in the engine | [Formats → Scene files → Loading order](../formats/scene.md#loading-order) |
| `Scene.load` is deferred to end-of-frame | [Known Limitations → Scene.load](../limitations.md#sceneload-is-deferred-to-end-of-frame) |
| Scene lifecycle (`onEnter` / `onExit`) | [Lifecycle → Scene lifecycle](lifecycle.md#scene-lifecycle) |

## Typical usage

    -- Restart the current scene
    Scene.load("home")

    -- Go to another scene on a button click
    button:addFunction(function() Scene.load("menu") end, Mouse.LEFT)

Do not assume the scene has changed on the next line after `Scene.load`. If you need to run code in the new scene, put it in a script's `start()` in that scene.