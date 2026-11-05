# Getting Started

This guide takes you from a clean Linux system to a running Jadidi project.

> Jadidi currently targets **Linux only**. Windows and macOS are not supported.

## Install dependencies

Pick the section for your distribution.

### Arch Linux

    sudo pacman -S --needed \
        base-devel cmake git pkgconf \
        sdl2 sdl2_image sdl2_ttf sdl2_mixer \
        lua lua54

### Fedora

    sudo dnf install -y \
        gcc-c++ cmake git pkgconf-pkg-config \
        SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL2_mixer-devel \
        lua-devel

### Debian / Ubuntu

    sudo apt update
    sudo apt install -y \
        build-essential cmake git pkg-config \
        libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev \
        liblua5.4-dev

### sol2

Jadidi uses the header-only sol2 library. Install it once per system:

    git clone https://github.com/ThePhD/sol2.git
    cd sol2
    mkdir build && cd build
    cmake ..
    make
    sudo make install

## Build the engine

    git clone https://github.com/MHoseinJ/jadidi.git
    cd jadidi
    mkdir build && cd build
    cmake ..
    make

The build produces the `jadidi` executable. To run a project, place that executable in the project directory (see below).

> The recommended installation path is `jadidi-hub`, which handles dependencies, sol2, engine builds, and project creation. The manual steps above are equivalent.

## Create a project

A runnable Jadidi project has this layout:

    MyGame/
    ├── config.json
    ├── icon.bmp
    ├── jadidi                 (the engine executable)
    ├── Fonts/
    │   └── font.ttf
    ├── Scenes/
    │   └── home.json
    ├── Scripts/
    └── Shaders/
        ├── sprite.frag
        └── sprite.vert

The engine reads `config.json` and `Scenes/home.json` at startup. `Shaders/` is used by the OpenGL renderer. `Fonts/` holds TTF files for `Text` components. `Scripts/` holds Lua files.

### config.json

A minimal configuration:

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

- `renderer` is `"opengl"` or `"sdl"`. `"opengl"` requests an OpenGL 3.3 core context.
- `fullscreen` switches to desktop fullscreen; `width` and `height` are then ignored and the display size is used.
- If `config.json` is missing, the engine writes a default one and continues.

See [Formats — config](formats/config.md) for the full field reference.

### Scenes/home.json

The engine loads the scene named `home` at startup. The scene file is `Scenes/home.json`. A minimal scene with no objects:

    { "objects": [] }

Each entry in `objects` is an object definition. A minimal object with a Lua script:

    {
      "objects": [
        {
          "name": "Player",
          "lua:Scripts/player.lua": {}
        }
      ]
    }

This creates one object named `Player` and attaches the Lua script at `Scripts/player.lua`. The empty `{}` is the data passed to the script's optional `init(data)` function.

See [Formats — scene](formats/scene.md) for the full schema.

### Scripts/player.lua

A minimal script that rotates its object:

    function start()
        Log.print("Player ready")
    end

    function update(dt)
        owner.transform.rotation = owner.transform.rotation + dt * 45
    end

The engine runs `start()` once, then `update(dt)` every frame. `owner` is the `GameObject` handle for the object the script is attached to. See [Lifecycle](api/lifecycle.md) for all available hooks.

## Run

From the project directory:

    ./jadidi

The engine reads `config.json`, creates the window, loads `Scenes/home.json`, and starts the main loop.

## Where to go next

- [Engine Overview](api/engine.md) — global namespaces and objects available in Lua
- [Lifecycle](api/lifecycle.md) — initialization, main loop, scene loading, and script hooks
- [API Reference](api/README.md) — components, transforms, and other types
- [Formats](formats/README.md) — `config.json` and scene file schemas
- [Examples](examples/README.md) — complete projects