#include "LuaBindings.h"
#include "iostream"
#include "LuaApi.h"
#include "core/Log.h"
#include "scene/GameObject.h"
#include "utils/FileSystem.h"

sol::state lua;
std::vector<LuaObject> scripts;

int test(int a, int b) {
    return a + b;
}

void Lua::init() {
    lua.open_libraries(sol::lib::base);
    gameLog("[LUA] initialized successfully", INFO);

    lua.set_function("test", &test);
}

void Lua::loadSceneScripts(const std::string& sceneName) {
    scripts.clear();
    lua.collect_garbage();

    lua.set_function("testf", LuaApi::debugger);

    lua.new_usertype<Vector2>(
        "vector2",
        "x", &Vector2::x,
        "y", &Vector2::y,
        "move", &Vector2::move,
        "set", &Vector2::set
    );
    lua["vector2"]["set"] = &Vector2::set;

    lua.new_usertype<GameObject>(
        "GameObject",
        "id", &GameObject::id,
        "name", &GameObject::name,
        "transform", &GameObject::transform,
        "sprite", &GameObject::sprite
    );

    lua.new_usertype<Transform>(
        "transform",
        "position", &Transform::position,
        "scale", &Transform::scale
    );

    LuaApi::bindKeys(lua);

    auto control = lua["Control"].get_or_create<sol::table>();

    // control.set_function("find", [this](const sol::lua_value &name_or_id) -> GameObject& {
    //     if (name_or_id.is<std::string>()) {
    //         return LuaApi::getObject(name_or_id);
    //     }
    //     if (name_or_id.is<int>()) {
    //         return LuaApi::getObject(name_or_id);
    //     }
    //     gameLog("Invalid argument type for find function", LogType::ERROR);
    //     return SceneManager::getCurrentScene()[0];
    //     ;
    // });

    control.set_function("findObject", &LuaApi::getObject);

    control.set_function("movePos", &LuaApi::moveObjectPosition);
    control.set_function("setPos", &LuaApi::setObjectPosition);
    control.set_function("getPos", &LuaApi::getObjectPosition);
    control.set_function("setScale", &LuaApi::setObjectScale);

    auto log = lua["Log"].get_or_create<sol::table>();

    log.set_function("print", &LuaApi::print);
    log.set_function("debug", &LuaApi::debug);
    log.set_function("info",  &LuaApi::info);
    log.set_function("warn",  &LuaApi::warn);
    log.set_function("error", &LuaApi::error);
    log.set_function("clear", &LuaApi::clear);

    auto Scene = lua["Scene"].get_or_create<sol::table>();

    Scene.set_function("set", &LuaApi::switchScene);

    auto app = lua["Application"].get_or_create<sol::table>();
    app.set_function("exit", &LuaApi::exit);

    auto input = lua["Input"].get_or_create<sol::table>();

    // keyboard
    input.set_function("keyPressed",
        sol::overload(
            static_cast<bool(*)(int)>(&LuaApi::isKeyPressed),
            static_cast<bool(*)(const std::string&)>(&LuaApi::isKeyPressed)
        )
    );

    input.set_function("keyDown",
        sol::overload(
            static_cast<bool(*)(int)>(&LuaApi::isKeyDown),
            static_cast<bool(*)(const std::string&)>(&LuaApi::isKeyDown)
        )
    );

    input.set_function("keyUp",
        sol::overload(
            static_cast<bool(*)(int)>(&LuaApi::isKeyUp),
            static_cast<bool(*)(const std::string&)>(&LuaApi::isKeyUp)
        )
    );

    // mouse
    input.set_function("mousePressed",
        sol::overload(
            static_cast<bool(*)(int)>(&LuaApi::isMousePressed),
            static_cast<bool(*)(const std::string&)>(&LuaApi::isMousePressed)
        )
    );

    input.set_function("mouseDown",
        sol::overload(
            static_cast<bool(*)(int)>(&LuaApi::isMouseDown),
            static_cast<bool(*)(const std::string&)>(&LuaApi::isMouseDown)
        )
    );

    input.set_function("mouseUp",
        sol::overload(
            static_cast<bool(*)(int)>(&LuaApi::isMouseUp),
            static_cast<bool(*)(const std::string&)>(&LuaApi::isMouseUp)
        )
    );

    input.set_function("mousePos", &LuaApi::getMousePosition);

    auto scriptsNames = fs::listFiles("Scripts");

    for (const auto& script : scriptsNames) {
        sol::environment env(lua, sol::create, lua.globals());

        sol::load_result chunk = lua.load_file(script);
        if (!chunk.valid()) {
            sol::error err = chunk;
            gameLog("[LUA] failed to load: " + script + " -> " + std::string(err.what()), ERROR);
            continue;
        }

        sol::protected_function pf = chunk;
        sol::protected_function_result result = pf(env);
        if (!result.valid()) {
            sol::error err = result;
            gameLog("[LUA] runtime error in: " + script + " -> " + std::string(err.what()), ERROR);
            continue;
        }

        sol::optional<std::string> maybeTarget = env["RUN_IN_SCENE"];
        if (!maybeTarget) continue;
        if (*maybeTarget != sceneName) continue;

        sol::function startFn = env["start"];
        sol::function updateFn = env["update"];

        LuaObject obj;
        obj.env = std::move(env);
        obj.start = std::move(startFn);
        obj.update = std::move(updateFn);

        scripts.push_back(std::move(obj));

        gameLog("[LUA] loaded script: " + script, INFO);
    }
}



void Lua::callStartLua() {
    if (scripts.empty()) return;

    LuaObject& obj = scripts[0];

    for (auto& pair : obj.env) {
        sol::object key = pair.first;
        sol::object value = pair.second;

        std::string keyStr = key.as<std::string>();
        std::string typeStr = (value.get_type() == sol::type::function) ? "function" : "value";

        std::cout << keyStr << " : " << typeStr << std::endl;
        std::cout << "im still alive" << std::endl;
    }

    for (auto & script : scripts) {
        if (!script.start.valid() || script.start.get_type() != sol::type::function)
            continue;

        sol::protected_function pf = script.start;
        sol::set_environment(script.env, pf);
        sol::protected_function_result result = pf();
        if (!result.valid()) {
            sol::error err = result;
            gameLog("[LUA] start error: " + std::string(err.what()), ERROR);
        }
    }
}

void Lua::callUpdateLua(float dt) {
    for (auto& script : scripts) {
        if (!script.update.valid() || script.update.get_type() != sol::type::function)
            continue;

        sol::protected_function pf = script.update;
        sol::set_environment(script.env, pf);
        sol::protected_function_result result = pf(dt);
        if (!result.valid()) {
            sol::error err = result;
            gameLog("[LUA] error: " + std::string(err.what()), ERROR);
        }
    }
}
