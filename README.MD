# Jadidi GameEngine

this engine works only on linux because i dont have time to build it for window or mac.
so you need to install libraries for linux. here is the command to install it

```bash

# for Arch Linux
sudo pacman -S sdl
sudo pacman -S lua
sudo pacman -S cmake

# for Fedora Linux
sudo dnf install lua lua-devel cmake
sudo dnf install SDL2 SDL2-devel SDL2_image SDL2_image-devel  SDL2_gfx  SDL2_gfx-devel  SDL2_ttf SDL2_ttf-devel

# sol2 installation
git clone https://github.com/ThePhD/sol2.git &&
cd sol2 &&
mkdir build &&
cd build &&
cmake .. &&
make &&
sudo make install

# also its preferred to compile the engine yourself
git clone https://gitea.com/JadidianMH/jadidi.git
mkdir jadidi/build
cd jadidi/build
cmake ..
make
```

then you're good to go!
just run the engine!

```bash
./jadidi
```

## Usage

all LUA apis are available in api.d.lua!
