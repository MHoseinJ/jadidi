#include "LuaApi.h"

#include <unordered_set>

#include "LuaBindings.h"
#include "core/Input.h"
#include "core/Log.h"
#include "render/TextureManager.h"
#include "scene/SceneManager.h"

#include "component/Animator.h"
#include "component/Audio.h"
#include "component/Button.h"
#include "component/Collider.h"
#include "component/Rigidbody.h"
#include "component/Sprite.h"
#include "component/Text.h"

// logging

void LuaApi::print(const std::string& str) {
    gameLog(str, LogType::PRINT);
}
void LuaApi::debug(const std::string& str) {
    gameLog(str, LogType::DEBUG);
}
void LuaApi::info(const std::string& str) {
    gameLog(str, LogType::INFO);
}
void LuaApi::warn(const std::string& str) {
    gameLog(str, LogType::WARNING);
}
void LuaApi::error(const std::string& str) {
    gameLog(str, LogType::ERROR);
}

void LuaApi::clear() {
    clearAllLogs();
}

void LuaApi::switchScene(const std::string& name) {
    auto& sm = SceneManager::getInstance();
    sm.loadScene(name);
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

// key binder

static bool is_lua_keyword(const std::string& s) {
    static const std::unordered_set<std::string> keywords = {
        "and", "break", "do",  "else", "elseif", "end",    "false",  "for",  "function", "goto",  "if",
        "in",  "local", "nil", "not",  "or",     "repeat", "return", "then", "true",     "until", "while"};
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

    out.erase(std::unique(out.begin(), out.end(), [](char a, char b) { return a == '_' && b == '_'; }), out.end());

    while (!out.empty() && out.front() == '_')
        out.erase(out.begin());
    while (!out.empty() && out.back() == '_')
        out.pop_back();

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

    mouse["LEFT"] = SDL_BUTTON_LEFT;
    mouse["RIGHT"] = SDL_BUTTON_RIGHT;
    mouse["MIDDLE"] = SDL_BUTTON_MIDDLE;
    mouse["X1"] = SDL_BUTTON_X1;
    mouse["X2"] = SDL_BUTTON_X2;

    lua["Mouse"] = mouse;
}

// component management for lua ( cuz lua is a little bit stupid to understand templates LOL )

Component* LuaApi::addComponent(GameObject& go, const std::string& name) {
    auto comp = Factory::instance().create(name);
    if (!comp) {
        gameLog("[Lua] Unknown component type: " + name, ERROR);
        return nullptr;
    }

    if (typeid(*comp) == typeid(Transform)) {
        gameLog("[Lua] Cannot add Transform manually", ERROR);
        return nullptr;
    }

    comp->owner = &go;

    const auto type = std::type_index(typeid(*comp));
    go.components[type] = std::move(comp);

    go.components[type]->OnCreate();
    return go.components[type].get();
}

Component* LuaApi::getComponent(GameObject& go, const std::string& name) {
    if (name == "transform" || name == "Transform") {
        return &go.transform;
    }

    static const std::unordered_map<std::string, std::type_index> typeMap = {{"sprite", typeid(Sprite)},
                                                                             {"Sprite", typeid(Sprite)},
                                                                             {"animator", typeid(Animator)},
                                                                             {"Animator", typeid(Animator)},
                                                                             {"rigidbody", typeid(Rigidbody)},
                                                                             {"Rigidbody", typeid(Rigidbody)},
                                                                             {"boxCollider", typeid(BoxCollider)},
                                                                             {"BoxCollider", typeid(BoxCollider)},
                                                                             {"text", typeid(Text)},
                                                                             {"Text", typeid(Text)},
                                                                             {"button", typeid(Button)},
                                                                             {"Button", typeid(Button)},
                                                                             {"audio", typeid(Audio)},
                                                                             {"Audio", typeid(Audio)}};

    const auto it = typeMap.find(name);
    if (it == typeMap.end()) {
        gameLog("[Lua] Unknown component name: '" + name + "'", ERROR);
        return nullptr;
    }

    const auto compIt = go.components.find(it->second);
    if (compIt == go.components.end()) {
        return nullptr;
    }

    return compIt->second.get();
}

sol::object LuaApi::LuaJSON(nlohmann::json& json) {

    if (json.is_object()) {
        sol::table table = lua.create_table();
        for (auto& [key, value] : json.items()) {
            table[key] = LuaJSON(value);
        }

        return table;
    }

    if (json.is_array()) {
        sol::table table = lua.create_table();
        size_t index = 1;
        for (auto& item : json) {
            table[index++] = LuaJSON(item);
        }
        return table;
    }

    if (json.is_number())
        return sol::make_object(lua, json.get<double>());
    if (json.is_string())
        return sol::make_object(lua, json.get<std::string>());
    if (json.is_boolean())
        return sol::make_object(lua, json.get<bool>());
    if (json.is_null())
        return sol::nil;

    return sol::nil;
}

nlohmann::json LuaApi::LuaJSON(const sol::object& obj) {
    switch (obj.get_type()) {
    case sol::type::table: {
        sol::table t = obj;
        nlohmann::json j;

        bool isArray = true;
        int index = 1;

        for (auto& [key, value] : t) {
            if (key.get_type() != sol::type::number || key.as<int>() != index++) {
                isArray = false;
                break;
            }
        }

        if (isArray) {
            size_t idx = 1;
            while (true) {
                sol::object value = t[idx];
                if (!value.valid() || value == sol::nil)
                    break;
                j.push_back(LuaJSON(value));
                ++idx;
            }
        } else {
            for (auto& [key, value] : t) {
                if (key.is<std::string>()) {
                    j[key.as<std::string>()] = LuaJSON(value);
                } else if (key.is<int>()) {
                    j[std::to_string(key.as<int>())] = LuaJSON(value);
                } else if (key.is<double>()) {
                    j[std::to_string(key.as<double>())] = LuaJSON(value);
                } else {
                    gameLog("Unsupported key type: " + key.as<std::string>(), ERROR);
                }
            }
        }

        return j;
    }

    case sol::type::string:
        return obj.as<std::string>();
    case sol::type::number:
        return obj.as<double>();
    case sol::type::boolean:
        return obj.as<bool>();
    case sol::type::nil:
        return nullptr;

    default:
        return nullptr;
    }
}

Vector2 LuaApi::getScreenSize() {
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    return Vector2(static_cast<float>(width), static_cast<float>(height));
}

sol::function LuaApi::getFunctionByName(const std::string& name) {
    for (auto& script : scripts) {
        sol::object result = script.env[name];

        if (result.valid() && result.is<sol::function>()) {
            return result.as<sol::function>();
        }
    }
    gameLog(std::to_string(scripts.size()), INFO);

    gameLog("[ENGINE]: no function found in lua \"" + name + "\"", ERROR);
    return sol::nil;
}
