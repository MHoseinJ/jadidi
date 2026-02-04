#include "LuaApi.h"

#include <unordered_set>

#include "LuaBindings.h"
#include "core/Log.h"
#include "scene/SceneManager.h"
#include "core/Input.h"

// logging

void LuaApi::print(const std::string& str)  { gameLog(str, LogType::PRINT); }
void LuaApi::debug(const std::string& str)  { gameLog(str, LogType::DEBUG); }
void LuaApi::info (const std::string& str)  { gameLog(str, LogType::INFO); }
void LuaApi::warn (const std::string& str)  { gameLog(str, LogType::WARNING); }
void LuaApi::error(const std::string& str)  { gameLog(str, LogType::ERROR); }

void LuaApi::clear() {
    clearAllLogs();
}

// game object

// GameObject &LuaApi::getObject(const std::string &name) {
//     return SceneManager::findGameObjectWithName(name);
// }
//
// void LuaApi::moveObjectPosition(const std::string& name, Vector2 vector) {
//     GameObject* object = &SceneManager::findGameObjectWithName(name);
//     if (!object) {
//         gameLog("object not found: " + name, LogType::ERROR);
//         return;
//     }
//     object->transform.position.x = vector.x;
//     object->transform.position.y = vector.y;
// }
//
// void LuaApi::setObjectPosition(const std::string& name, Vector2 vector) {
//     GameObject* object = &SceneManager::findGameObjectWithName(name);
//     if (!object) {
//         gameLog("object not found: " + name, LogType::ERROR);
//         return;
//     }
//     object->transform.position = vector;
// }
//
// void LuaApi::setObjectScale(const std::string& name, Vector2 vector) {
//     GameObject* object = &SceneManager::findGameObjectWithName(name);
//     if (!object) {
//         gameLog("object not found: " + name, LogType::ERROR);
//         return;
//     }
//     object->transform.scale = vector;
// }
//
// Vector2 LuaApi::getObjectPosition(const std::string& name) {
//     const GameObject* object = &SceneManager::findGameObjectWithName(name);
//     if (!object) {
//         gameLog("object not found: " + name, LogType::ERROR);
//         return Vector2{0, 0};
//     }
//     return object->transform.position;
// }

void LuaApi::switchScene(const std::string& name) {
    SceneManager::loadSceneJson(name);
    Lua::loadSceneScripts(name);
}

void LuaApi::exit() {
    Input::Quit();
}

// input management

bool LuaApi::isKeyPressed(int scancode) {
    return Input::IsKeyPressed(static_cast<SDL_Scancode>(scancode));
}

bool LuaApi::isKeyDown(int scancode) {
    return Input::IsKeyDown(static_cast<SDL_Scancode>(scancode));
}

bool LuaApi::isKeyUp(int scancode) {
    return Input::IsKeyUp(static_cast<SDL_Scancode>(scancode));
}

bool LuaApi::isMousePressed(int button) {
    return Input::IsMouseButtonPressed(button);
}

bool LuaApi::isMouseDown(int button) {
    return Input::IsMouseButtonDown(button);
}

bool LuaApi::isMouseUp(int button) {
    return Input::IsMouseButtonUp(button);
}

// input string overload

bool LuaApi::isKeyPressed(const std::string& key) {
    return Input::IsKeyPressed(key);
}

bool LuaApi::isKeyDown(const std::string& key) {
    return Input::IsKeyDown(key);
}

bool LuaApi::isKeyUp(const std::string& key) {
    return Input::IsKeyUp(key);
}

bool LuaApi::isMousePressed(const std::string& button) {
    return Input::IsMouseButtonPressed(button);
}

bool LuaApi::isMouseDown(const std::string& button) {
    return Input::IsMouseButtonDown(button);
}

bool LuaApi::isMouseUp(const std::string& button) {
    return Input::IsMouseButtonUp(button);
}

// mouse

Vector2 LuaApi::getMousePosition() {
    int x, y;
    Input::GetMousePosition(x, y);
    return Vector2{ static_cast<float>(x), static_cast<float>(y) };
}

// key binder

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

    out.erase(
        std::unique(out.begin(), out.end(),
            [](char a, char b) { return a == '_' && b == '_'; }),
        out.end()
    );

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

void LuaApi::bindKeys(sol::state& lua) {
    sol::table key = lua.create_table();

    for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
        const char* raw = SDL_GetScancodeName(static_cast<SDL_Scancode>(i));
        if (!raw || raw[0] == '\0')
            continue;

        std::string luaName = sanitize(raw);
        key[luaName] = i;
    }

    lua["Key"] = key;
}

void LuaApi::bindMouse(sol::state& lua) {
    sol::table mouse = lua.create_table();

    mouse["LEFT"]   = std::to_string(SDL_BUTTON_LEFT);   // 1
    mouse["RIGHT"]  = std::to_string(SDL_BUTTON_RIGHT);  // 2
    mouse["MIDDLE"] = std::to_string(SDL_BUTTON_MIDDLE); // 3
    mouse["X1"]     = std::to_string(SDL_BUTTON_X1);     // 4
    mouse["X2"]     = std::to_string(SDL_BUTTON_X2);     // 5

    lua["Mouse"] = mouse;
}
