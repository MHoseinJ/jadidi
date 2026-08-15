#include "LuaBindings.h"
#include "GameObjectHandle.h"
#include "core/Engine.h"
#include "core/Input.h"
#include "core/Log.h"
#include "scene/SceneManager.h"
#include "utils/FileSystem.h"

sol::state lua;
std::vector<LuaObject> scripts;
std::string pendingSceneLoad = "";

void Lua::init() {
    LuaBindings::bindCore(lua);
    LuaBindings::bindMath(lua);
    LuaBindings::bindInput(lua);
    LuaBindings::bindDebug(lua);
    LuaBindings::bindAsset(lua);
    LuaBindings::bindState(lua);
    LuaBindings::bindECS(lua);
    LuaBindings::bindScene(lua);
}

void Lua::loadSceneScripts(const std::string& sceneName) {
    scripts.clear();
    lua.collect_garbage();
    
    const auto scriptsNames = fs::listFiles("Scripts");
    for (const auto& script : scriptsNames) {
        sol::environment env(lua, sol::create, lua.globals());
        auto chunk = lua.load_file(script);
        if (!chunk.valid()) {
            gameLog("[LUA] load failed: " + script, ERROR);
            continue;
        }
        
        sol::protected_function pf = chunk;
        auto result = pf(env);
        if (!result.valid()) {
            gameLog("[LUA] runtime error: " + script, ERROR);
            continue;
        }
        
        sol::optional<std::string> target = env["RUN_IN_SCENE"];
        if (!target.has_value() || target.value() != sceneName)
            continue;
            
        LuaObject obj;
        obj.env = std::move(env);
        obj.start = obj.env["start"];
        obj.update = obj.env["update"];
        scripts.push_back(std::move(obj));
    }
}

void Lua::callStartLua() {
    if (scripts.empty()) return;
    
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
    
    if (!pendingSceneLoad.empty()) {
        SceneManager::getInstance().loadScene(pendingSceneLoad);
        Lua::loadSceneScripts(pendingSceneLoad);
        callStartLua();
        pendingSceneLoad = "";
    }
}