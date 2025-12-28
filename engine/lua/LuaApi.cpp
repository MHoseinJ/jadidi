#include "LuaApi.h"

#include "LuaBindings.h"
#include "core/Log.h"
#include "core/Engine.h"
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

std::string LuaApi::getObject(const std::string& name) {
    for (auto& check : currentScene) {
        if (check.name == name)
            return name;
    }
    gameLog("game object not found: " + name, LogType::ERROR);
    return "";
}

void LuaApi::moveObjectPosition(const std::string& name, Vector2 vector) {
    GameObject* object = SceneManager::findGameObjectWithName(name);
    if (!object) {
        gameLog("object not found: " + name, LogType::ERROR);
        return;
    }
    object->transform.position.x = vector.x;
    object->transform.position.y = vector.y;
}

void LuaApi::setObjectPosition(const std::string& name, Vector2 vector) {
    GameObject* object = SceneManager::findGameObjectWithName(name);
    if (!object) {
        gameLog("object not found: " + name, LogType::ERROR);
        return;
    }
    object->transform.position = vector;
}

void LuaApi::setObjectScale(const std::string& name, Vector2 vector) {
    GameObject* object = SceneManager::findGameObjectWithName(name);
    if (!object) {
        gameLog("object not found: " + name, LogType::ERROR);
        return;
    }
    object->transform.scale = vector;
}

Vector2 LuaApi::getObjectPosition(const std::string& name) {
    const GameObject* object = SceneManager::findGameObjectWithName(name);
    if (!object) {
        gameLog("object not found: " + name, LogType::ERROR);
        return Vector2{0, 0};
    }
    return object->transform.position;
}

// scene

void LuaApi::switchScene(const std::string& name) {
    SceneManager::loadSceneJson(name);
    Lua::loadSceneScripts(name);
}

void LuaApi::exit(int /*code*/) {
    quit();
}

// number

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
    return Vector2{ (float)x, (float)y };
}

// key binder

static std::string sanitize(const std::string& s) {
    std::string out = s;
    for (char& c : out) {
        if (c == ' ' || c == '-' || c == '/')
            c = '_';
    }
    if (!out.empty() && std::isdigit(static_cast<unsigned char>(out[0]))) {
        out = "_" + out;
    }
    return out;
}

void LuaApi::bindKeys(sol::state& lua) {
    sol::table key = lua["Key"].get_or_create<sol::table>();

    for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
        const char* raw = SDL_GetScancodeName(static_cast<SDL_Scancode>(i));
        if (!raw || raw[0] == '\0')
            continue;

        key[sanitize(raw)] = i;
    }
}