# Jadidi Game Engine

Jadidi is a small 2D game engine written in C++17 and scripted with Lua.

This engine currently targets Linux only. Windows and macOS are not supported
at this time.

## Features

- SDL2 based rendering
- Lua 5.4 scripting
- Scene based workflow
- JSON scene and config files
- SDL2_image, SDL2_ttf, and SDL2_mixer integration

## Recommended installation

The recommended way to install and manage the engine is through
[jadidi-hub](https://github.com/MHoseinJ/jadidi-hub).

It handles dependencies, sol2, engine builds, and project creation.

    jadidi-hub install-deps
    jadidi-hub install-sol2
    jadidi-hub engine-sync https://github.com/MHoseinJ/jadidi.git
    jadidi-hub engine-build
    jadidi-hub project-new ~/Projects/MyGame --git-init

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
    └── Scripts/

Example `config.json`:

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

Example `Scenes/home.json`:

    { "objects": [] }

Run the engine from the project directory:

    ./jadidi

## Scripting

Lua API definitions for editor autocompletion are available in the
`ide autocompletion` directory.

## Special thanks

- [ThePhD](https://github.com/ThePhD) for sol2
- [nlohmann](https://github.com/nlohmann) for the JSON library
- [SDL](https://github.com/libsdl-org) for the SDL libraries

## License

GNU Affero General Public License