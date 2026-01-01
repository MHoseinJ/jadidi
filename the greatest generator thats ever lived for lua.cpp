#include <SDL.h>
#include <iostream>
#include <string>
#include <algorithm>

static std::string sanitize(const std::string& s) {
    std::string out = s;
    for (char& c : out) {
        if (c == ' ' || c == '-' || c == '/') c = '_';
    }
    if (!out.empty() && std::isdigit(out[0])) {
        out = "_" + out;
    }
    return out;
}

int main() {
    std::cout << "---@enum Key\n";
    std::cout << "Key = {\n";

    for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
        const char* raw = SDL_GetScancodeName(static_cast<SDL_Scancode>(i));
        if (!raw || raw[0] == '\0') continue;

        std::string name(raw);
        std::string luaName = sanitize(name);

        std::cout
            << "    "
            << luaName
            << " = \""
            << name
            << "\",\n";
    }

    std::cout << "}\n";
    return 0;
}