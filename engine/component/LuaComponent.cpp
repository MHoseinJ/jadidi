#include "component/LuaComponent.h"
#include "core/Log.h"
#include "lua/LuaBindings.h"
#include "scene/GameObject.h"

extern sol::state lua;

void LuaComponent::OnCreate() {
    if (!loadScript()) {
        gameLog("[LuaComponent] Failed to load script: " + scriptPath, ERROR);
        return;
    }
    
    if (onStart.valid() && onStart.get_type() == sol::type::function) {
        sol::protected_function pf = onStart;
        sol::set_environment(env, pf);
        auto result = pf();
        if (!result.valid()) {
            sol::error err = result;
            gameLog("[LuaComponent] start error in " + scriptPath + ": " + std::string(err.what()), ERROR);
        }
    }
}

void LuaComponent::OnDestroy() {
    if (onDestroyFunc.valid() && onDestroyFunc.get_type() == sol::type::function) {
        sol::protected_function pf = onDestroyFunc;
        sol::set_environment(env, pf);
        auto result = pf();
        if (!result.valid()) {
            sol::error err = result;
            gameLog("[LuaComponent] onDestroy error in " + scriptPath + ": " + std::string(err.what()), ERROR);
        }
    }
}

void LuaComponent::Update(float dt) {
    if (onUpdate.valid() && onUpdate.get_type() == sol::type::function) {
        sol::protected_function pf = onUpdate;
        sol::set_environment(env, pf);
        auto result = pf(dt);
        if (!result.valid()) {
            sol::error err = result;
            gameLog("[LuaComponent] update error in " + scriptPath + ": " + std::string(err.what()), ERROR);
        }
    }
}

void LuaComponent::DeSerialize(const Json& j) {
    if (j.has("path")) {
        scriptPath = j.get<std::string>("path");
    }
    
    if (!loadScript()) {
        gameLog("[LuaComponent] Failed to load script during deserialize: " + scriptPath, ERROR);
        return;
    }
    
    sol::object initFunc = env["init"];
    if (initFunc.valid() && initFunc.is<sol::function>()) {
        sol::protected_function pf = initFunc.as<sol::function>();
        sol::set_environment(env, pf);
        
        sol::table params = lua.create_table();
        for (const auto& [key, value] : j.raw().items()) {
            if (key == "path") continue;
            params[key] = value;
        }
        
        auto result = pf(params);
        if (!result.valid()) {
            sol::error err = result;
            gameLog("[LuaComponent] init error in " + scriptPath + ": " + std::string(err.what()), ERROR);
        }
    }
}

void LuaComponent::OnCollisionEnter(GameObjectHandle other) {
    if (onCollisionEnterFunc.valid() && onCollisionEnterFunc.get_type() == sol::type::function) {
        sol::protected_function pf = onCollisionEnterFunc;
        sol::set_environment(env, pf);
        auto result = pf(other);
        if (!result.valid()) {
            sol::error err = result;
            gameLog("[LuaComponent] onCollisionEnter error: " + std::string(err.what()), ERROR);
        }
    }
}

void LuaComponent::OnCollisionExit(GameObjectHandle other) {
    if (onCollisionExitFunc.valid() && onCollisionExitFunc.get_type() == sol::type::function) {
        sol::protected_function pf = onCollisionExitFunc;
        sol::set_environment(env, pf);
        auto result = pf(other);
        if (!result.valid()) {
            sol::error err = result;
            gameLog("[LuaComponent] onCollisionExit error: " + std::string(err.what()), ERROR);
        }
    }
}

void LuaComponent::OnTriggerEnter(GameObjectHandle other) {
    if (onTriggerEnterFunc.valid() && onTriggerEnterFunc.get_type() == sol::type::function) {
        sol::protected_function pf = onTriggerEnterFunc;
        sol::set_environment(env, pf);
        auto result = pf(other);
        if (!result.valid()) {
            sol::error err = result;
            gameLog("[LuaComponent] onTriggerEnter error: " + std::string(err.what()), ERROR);
        }
    }
}

void LuaComponent::OnTriggerExit(GameObjectHandle other) {
    if (onTriggerExitFunc.valid() && onTriggerExitFunc.get_type() == sol::type::function) {
        sol::protected_function pf = onTriggerExitFunc;
        sol::set_environment(env, pf);
        auto result = pf(other);
        if (!result.valid()) {
            sol::error err = result;
            gameLog("[LuaComponent] onTriggerExit error: " + std::string(err.what()), ERROR);
        }
    }
}

bool LuaComponent::loadScript() {
    env = sol::environment(lua, sol::create, lua.globals());
    
    env["owner"] = owner;
    
    auto chunk = lua.load_file(scriptPath);
    if (!chunk.valid()) {
        gameLog("[LuaComponent] Failed to load file: " + scriptPath, ERROR);
        return false;
    }
    
    sol::protected_function pf = chunk;
    auto result = pf(env);
    if (!result.valid()) {
        sol::error err = result;
        gameLog("[LuaComponent] Runtime error in " + scriptPath + ": " + std::string(err.what()), ERROR);
        return false;
    }
    
    onStart = env["start"];
    onUpdate = env["update"];
    onDestroyFunc = env["onDestroy"];
    onCollisionEnterFunc = env["onCollisionEnter"];
    onCollisionExitFunc = env["onCollisionExit"];
    onTriggerEnterFunc = env["onTriggerEnter"];
    onTriggerExitFunc = env["onTriggerExit"];
    
    return true;
}