#pragma once
#include "Component.h"
#include <sol/sol.hpp>
#include <string>

struct LuaComponent : public Component {
    std::string scriptPath;
    sol::environment env;
    sol::function onStart;
    sol::function onUpdate;
    sol::function onDestroyFunc;
    sol::function onCollisionEnterFunc;
    sol::function onCollisionExitFunc;
    sol::function onTriggerEnterFunc;
    sol::function onTriggerExitFunc;
    
    LuaComponent() = default;
    ~LuaComponent() override = default;
    
    void OnCreate() override;
    void OnDestroy() override;
    void Update(float dt) override;
    void DeSerialize(const Json& j) override;
    void OnCollisionEnter(GameObjectHandle other) override;
    void OnCollisionExit(GameObjectHandle other) override;
    void OnTriggerEnter(GameObjectHandle other) override;
    void OnTriggerExit(GameObjectHandle other) override;
    
    std::string typeName() const override { return "lua:" + scriptPath; }
    static std::string staticTypeName(const std::string& path) { return "lua:" + path; }
    
private:
    bool loadScript();
};