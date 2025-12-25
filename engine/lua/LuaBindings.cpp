#include "LuaBindings.h"
#include "iostream"
#include "LuaApi.h"
#include "core/Log.h"
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

    lua.new_usertype<Vector2>(
        "vector2",
        "x", &Vector2::x,
        "y", &Vector2::y
    );

    auto gameObject = lua["GameObject"].get_or_create<sol::table>();

    gameObject.set_function("getObj", &LuaApi::getObject);
    gameObject.set_function("movePos", &LuaApi::moveObjectPosition);
    gameObject.set_function("setPos", &LuaApi::setObjectPosition);
    gameObject.set_function("getPos", &LuaApi::getObjectPosition);
    gameObject.set_function("setScale", &LuaApi::setObjectScale);

    auto log = lua["Log"].get_or_create<sol::table>();

    log.set_function("print", &LuaApi::print);
    log.set_function("debug", &LuaApi::debug);
    log.set_function("info",  &LuaApi::info);
    log.set_function("warn",  &LuaApi::warn);
    log.set_function("error", &LuaApi::error);
    log.set_function("clear", &LuaApi::clear);

    auto control = lua["Scene"].get_or_create<sol::table>();

    control.set_function("set", &LuaApi::switchScene);

    auto input = lua["Input"].get_or_create<sol::table>();

    input.set_function("keyPressed", &LuaApi::isKeyPressed);
    input.set_function("keyDown",    &LuaApi::isKeyDown);
    input.set_function("keyUp",      &LuaApi::isKeyUp);

    input.set_function("mousePressed", &LuaApi::isMousePressed);
    input.set_function("mouseDown",    &LuaApi::isMouseDown);
    input.set_function("mouseUp",      &LuaApi::isMouseUp);

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

    for (size_t i = 0; i < scripts.size(); ++i) {
        auto& script = scripts[i];
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