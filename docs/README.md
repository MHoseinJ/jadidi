# Jadidi Documentation

Jadidi is a small 2D game engine written in C++17 and scripted with **Lua 5.4**. It uses SDL2 for rendering, input, audio, and windowing; scenes and configuration are described in JSON; and game logic lives in Lua scripts attached to game objects.

> Jadidi currently targets **Linux only**. Windows and macOS are not supported.

## Features

- SDL2-based rendering (OpenGL 3.3 core, or SDL renderer)
- Lua 5.4 scripting via sol2
- Scene-based workflow with JSON scene files
- JSON configuration file
- 2D physics (box colliders, rigidbodies, triggers, raycasts)
- Sprite, text, animation, audio, and UI components
- Key and mouse input with named constants
- Global state table shared across scripts
- In-game log console (DEV_MODE)

## Installation

See [Getting Started](getting-started.md) for installing dependencies, building the engine, and creating your first project.

## Documentation map

| Section | Description |
|---------|-------------|
| [Getting Started](getting-started.md) | Build, run, and create your first project |
| [Engine Overview](api/engine.md) | Engine object, globals, and top-level namespaces |
| [Lifecycle](api/lifecycle.md) | Initialization, main loop, scenes, and per-object script hooks |
| [API Reference](api/README.md) | Lua API for components, objects, input, math, physics, and more |
| [Formats](formats/README.md) | `config.json` and scene JSON schemas |
| [Examples](examples/README.md) | Complete example projects |

## License

Jadidi is released under the GNU Affero General Public License.