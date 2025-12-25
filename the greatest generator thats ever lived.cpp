#include <SDL.h>
#include <iostream>
#include <string>

int main() {
    std::cout << "std::unordered_map<std::string, SDL_Scancode> keyMap = {\n";
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
        std::string name = SDL_GetScancodeName(static_cast<SDL_Scancode>(i));
        if (!name.empty()) {
            std::cout << "    {\"" << name << "\", SDL_Scancode(" << i << ")},\n";
        }
    }

    std::cout << "};\n";
}