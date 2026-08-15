#include "lua/LuaBindings.h"
#include "lua/GameObjectHandle.h"
#include "scene/SceneManager.h"
#include "scene/GameObject.h"
#include "core/Audio.h"
#include "core/Input.h"
#include "core/Log.h"
#include "lua/LuaApi.h"

void LuaBindings::bindScene(sol::state& lua) {
    // Engine Exit
    auto engine = lua["Engine"].get_or_create<sol::table>();
    engine.set_function("exit", []() {
        gameLog("Engine.exit() called - shutting down...", INFO);
        Input::Quit();
    });

    // Screen
    auto screen = lua["Screen"].get_or_create<sol::table>();
    screen.set_function("size", &LuaApi::getScreenSize);

    // Scene
    auto scene = lua["Scene"].get_or_create<sol::table>();
    scene.set_function("load", [](const std::string& name) {
        pendingSceneLoad = name; // دسترسی به extern
    });

    // Objects (GameObjectHandle logic)
    auto gameobject = lua["Objects"].get_or_create<sol::table>();
    
    gameobject.set_function("find",
        sol::overload(
            [](const std::string& name) -> sol::object {
                GameObject* go = SceneManager::getInstance().findGameObjectWithName(name);
                if (!go) return sol::nil;
                return sol::make_object(::lua, GameObjectHandle(go->id));
            },
            [](uint64_t id) -> sol::object {
                GameObject* go = SceneManager::getInstance().findGameObjectWithId(id);
                if (!go) return sol::nil;
                return sol::make_object(::lua, GameObjectHandle(go->id));
            }
        )
    );

    gameobject.set_function("create", [](const std::string& name) -> sol::object {
        GameObject& go = SceneManager::getInstance().createObject(name);
        return sol::make_object(::lua, GameObjectHandle(go.id));
    });

    gameobject.set_function("deleteById",
        sol::overload(
            [](uint64_t id) { SceneManager::getInstance().deleteObjectById(id); },
            [](GameObjectHandle handle) {
                if (GameObject* go = handle.resolve())
                    SceneManager::getInstance().deleteObjectById(go->id);
            }
        )
    );
    
    gameobject.set_function("deleteByName", [](const std::string& name){
        SceneManager::getInstance().deleteAllObjectsByName(name);
    });
    
    gameobject.set_function("deleteByTag", [](const std::string& tag){
        SceneManager::getInstance().deleteAllObjectsByTag(tag);
    });

    lua.set_function("isValid", [](sol::object obj) -> bool {
        if (!obj.is<GameObjectHandle>()) return false;
        GameObjectHandle handle = obj.as<GameObjectHandle>();
        return handle.isValid();
    });

    // Camera
    lua.new_usertype<Camera>("Camera",
        "transform", &Camera::transform,
        "zoom", &Camera::zoom
    );
    lua["Camera"] = &camera;

    // AudioSystem
    auto audio_system = lua["AudioSystem"].get_or_create<sol::table>();
    audio_system.set_function("load", [](const std::string& name, const std::string& path, const bool isMusic) {
        gameLog("Lua calling load: name='" + name + "', path='" + path + "'", INFO);
        AudioSystem::getInstance().LoadSound(name, path, isMusic);
    });
}