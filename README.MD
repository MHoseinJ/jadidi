FILE: README.md
<img src="logo.png" alt="Jadidi Game Engine Logo" width="180">

# Jadidi Game Engine

Jadidi is a small 2D game engine written in C++17 and scripted with Lua.

This engine currently targets Linux only. Windows and macOS are not supported
at this time.

> **Status:** alpha. APIs may change between releases. See the
> [release notes](https://github.com/MHoseinJ/jadidi/releases) and
> [known limitations](docs/limitations.md) before relying on a feature.

## Features

- SDL2 based rendering (OpenGL 3.3 or SDL renderer)
- Lua 5.4 scripting via sol2, with per-object script environments
- Scene based workflow with JSON scene and config files
- Parent/child hierarchy with local and world transforms
- 2D physics: box colliders, rigidbodies, triggers, raycasts
- Sprite, text, animation, audio, and UI components
- SDL2_image, SDL2_ttf, and SDL2_mixer integration

## Documentation

Full documentation lives in [`docs/`](docs/README.md):

| Section | Description |
|---------|-------------|
| [Getting Started](docs/getting-started.md) | Build, run, and create your first project |
| [Engine Overview](docs/api/engine.md) | Lua globals and namespaces |
| [Lifecycle](docs/api/lifecycle.md) | Startup, main loop, and script hooks |
| [API Reference](docs/api/README.md) | Lua API for objects, components, math, physics, and more |
| [Formats](docs/formats/README.md) | `config.json`, scene files, and animation files |
| [Known Limitations](docs/limitations.md) | Behaviors to design around in this release |

## Recommended installation

The recommended way to install and manage the engine is through
[jadidi-hub](https://github.com/MHoseinJ/jadidi-hub).

It handles dependencies, sol2, engine builds, and project creation.

    jadidi-hub install-deps
    jadidi-hub install-sol2
    jadidi-hub engine-sync https://github.com/MHoseinJ/jadidi.git
    jadidi-hub engine-build
    jadidi-hub project-new ~/Projects/MyGame --git-init

See [Getting Started](docs/getting-started.md) for the manual installation
path and a walkthrough of the project layout.

## Manual installation

If you prefer to build the engine manually, install the required system
packages first.

### Arch Linux

    sudo pacman -S --needed \
        base-devel \
        cmake \
        git \
        pkgconf \
        sdl2 \
        sdl2_image \
        sdl2_ttf \
        sdl2_mixer \
        lua \
        lua54

### Fedora

    sudo dnf install -y \
        gcc-c++ \
        cmake \
        git \
        pkgconf-pkg-config \
        SDL2-devel \
        SDL2_image-devel \
        SDL2_ttf-devel \
        SDL2_mixer-devel \
        lua-devel

### Debian

    sudo apt update
    sudo apt install -y \
        build-essential \
        cmake \
        git \
        pkg-config \
        libsdl2-dev \
        libsdl2-image-dev \
        libsdl2-ttf-dev \
        libsdl2-mixer-dev \
        liblua5.4-dev

### sol2

sol2 is required as a header-only library.

    git clone https://github.com/ThePhD/sol2.git
    cd sol2
    mkdir build
    cd build
    cmake ..
    make
    sudo make install

### Build the engine

    git clone https://github.com/MHoseinJ/jadidi.git
    cd jadidi
    mkdir build
    cd build
    cmake ..
    make

## Running a project

A runnable project needs this structure:

    MyGame/
    ├── config.json
    ├── icon.bmp
    ├── jadidi
    ├── Fonts/
    │   └── font.ttf
    ├── Scenes/
    │   └── home.json
    ├── Scripts/
    └── Shaders/
        ├── sprite.frag
        └── sprite.vert

Run the engine from the project directory:

    ./jadidi

For the contents of `config.json`, a minimal `Scenes/home.json`, a first
script, and a walkthrough of the layout, see
[Getting Started](docs/getting-started.md). For the file schemas, see
[Formats](docs/formats/README.md).

## Scripting

Game logic is written in Lua 5.4. Scripts are attached to game objects
through the scene file and receive `start()`, `update(dt)`, and
collision/trigger callbacks. The full Lua API is documented in
[`docs/api/`](docs/api/README.md).

## Special thanks

- [ThePhD](https://github.com/ThePhD) for sol2
- [nlohmann](https://github.com/nlohmann) for the JSON library
- [SDL](https://github.com/libsdl-org) for the SDL libraries

## License

This project is licensed under the GNU Affero General Public License.
