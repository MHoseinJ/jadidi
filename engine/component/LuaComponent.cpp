#include "component/LuaComponent.h"
#include "core/Log.h"
#include "lua/LuaBindings.h"
#include "scene/GameObject.h"
#include "lua/LuaApi.h"
#include <sol/forward.hpp>
#include <sol/types.hpp>

extern sol::state lua;

void LuaComponent::OnCreate() {
    // silence is golden
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
    if (!started) {
        started = true;

        if (onStart.valid() && onStart.get_type() == sol::type::function) {
            sol::protected_function pf = onStart;
            sol::set_environment(env, pf);

            auto result = pf();

            if (!result.valid()) {
                sol::error err = result;
                gameLog(
                    "[LuaComponent] start error in " +
                    scriptPath + ": " +
                    std::string(err.what()),
                    ERROR
                );
            }
        }
    }

    if (onUpdate.valid() && onUpdate.get_type() == sol::type::function) {
        sol::protected_function pf = onUpdate;
        sol::set_environment(env, pf);

        auto result = pf(dt);

        if (!result.valid()) {
            sol::error err = result;
            gameLog(
                "[LuaComponent] update error in " +
                scriptPath + ": " +
                std::string(err.what()),
                ERROR
            );
        }
    }
}

void LuaComponent::awake() {
    if (!loadScript()) {
        return;
    }

    sol::object localAwake = env.raw_get<sol::object>("awake");
    sol::object globalAwake = lua.globals()["awake"];

    sol::object awakeFunc = env["awake"];

    if (awakeFunc.valid() && awakeFunc.is<sol::function>()) {
        sol::protected_function pf =
            awakeFunc.as<sol::function>();

        sol::set_environment(env, pf);

        auto result = pf();

        if (!result.valid()) {
            sol::error err = result;
            gameLog(
                "[LuaComponent] awake error in " +
                scriptPath + ": " +
                std::string(err.what()),
                ERROR
            );
        }
    }
}

void LuaComponent::DeSerialize(const Json& j)
{
    initData = j.raw();

    sol::object initFunc = env["init"];

    if (!initFunc.valid() || !initFunc.is<sol::function>())
        return;

    sol::protected_function pf =
        initFunc.as<sol::function>();

    sol::set_environment(env, pf);

    sol::object luaTable =
        LuaApi::LuaJSON(initData);

    auto result = pf(luaTable);

    if (!result.valid()) {
        sol::error err = result;

        gameLog(
            "[LuaComponent] init error in " +
            scriptPath +
            ": " +
            std::string(err.what()),
            ERROR
        );
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
    
    if (!owner)
        return false;

    gameLog(
        "[LuaComponent] LOAD SCRIPT: " + scriptPath,
        WARNING
    );
    
    env["owner"] = GameObjectHandle(owner->id);
    
    auto chunk = lua.load_file(scriptPath);
    if (!chunk.valid()) {
        gameLog("[LuaComponent] Failed to load file: " + scriptPath, ERROR);
        return false;
    }
    
    sol::protected_function pf = chunk;
    sol::set_environment(env, pf);
    
    auto result = pf();
    
    if (!result.valid()) {
        sol::error err = result;
        gameLog(
            "[LuaComponent] Runtime error in " +
            scriptPath + ": " +
            std::string(err.what()),
            ERROR
        );
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