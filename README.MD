this engine works only on linux because i dont have time to build it for window or mac.
so you need to install libraries for linux. here is the command to install it

```bash
sudo pacman -S sdl
# for installing sol2
sudo pacman -S lua

sudo pacman -S cmake

git clone https://github.com/ThePhD/sol2.git
cd sol2

mkdir build
cd build
cmake ..
make
sudo make install
```

them you're good to go!
just run the engine!
```bash
./jadidi
```