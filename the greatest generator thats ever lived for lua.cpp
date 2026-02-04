#include <SDL.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_set>

static bool is_lua_keyword(const std::string& s) {
    static const std::unordered_set<std::string> keywords = {
        "and","break","do","else","elseif","end","false","for",
        "function","goto","if","in","local","nil","not","or",
        "repeat","return","then","true","until","while"
    };
    return keywords.count(s) > 0;
}

static std::string sanitize(const std::string& s) {
    std::string out;
    out.reserve(s.size());

    for (char c : s) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            out += std::toupper(c);
        } else {
            out += '_';
        }
    }

    // collapse multiple underscores
    out.erase(
        std::unique(out.begin(), out.end(),
            [](char a, char b) { return a == '_' && b == '_'; }),
        out.end()
    );

    // trim underscores
    while (!out.empty() && out.front() == '_') out.erase(out.begin());
    while (!out.empty() && out.back() == '_') out.pop_back();

    if (out.empty() || std::isdigit(out[0])) {
        out = "_" + out;
    }

    if (is_lua_keyword(out)) {
        out = out + "_KEY";
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
            << i
            << "\",\n";
    }

    std::cout << "}\n";
    return 0;
}