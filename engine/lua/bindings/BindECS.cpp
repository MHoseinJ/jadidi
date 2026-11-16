#include "component/Animator.h"
#include "component/Audio.h"
#include "component/Button.h"
#include "component/Collider.h"
#include "component/Component.h"
#include "component/Rigidbody.h"
#include "component/Sprite.h"
#include "component/Text.h"
#include "core/Engine.h"
#include "lua/GameObjectHandle.h"
#include "lua/LuaApi.h"
#include "lua/LuaBindings.h"
#include <sol/error.hpp>

void LuaBindings::bindECS(sol::state& lua) {

    lua["registerFunction"] = [](const std::string& name, const sol::function& func) {
        LuaApi::registerGlobalFunction(name, func);
    };

    lua.new_usertype<Component>("Component");

    lua.new_usertype<Transform>("Transform",
        "position", sol::property(
            [](Transform& self) { return PositionProxy{&self}; },
            [](Transform& self, const Vector2& v) { self.setLocalPosition(v); }
        ),
        "scale", sol::property(
            [](Transform& self) { return ScaleProxy{&self}; },
            [](Transform& self, const Vector2& v) { self.setLocalScale(v); }
        ),
        "rotation", sol::property(
            [](Transform& self) { return self.getLocalRotation(); },
            [](Transform& self, float v) { self.setLocalRotation(v); }
        ),
        "worldPosition", sol::property([](Transform& self) { return self.getWorldPosition(); }),
        "worldScale",    sol::property([](Transform& self) { return self.getWorldScale(); }),
        "worldRotation", sol::property([](Transform& self) { return self.getWorldRotation(); })
    );

    lua.new_usertype<BoxCollider>("BoxCollider", sol::base_classes, sol::bases<Component>(),
        "size", &BoxCollider::size,
        "isTrigger", &BoxCollider::isTrigger,
        "rebuildBody", &BoxCollider::rebuildBody,
        "overlap",
        sol::overload(
            [](BoxCollider& self, BoxCollider& other) { return IsColliding(&self, &other); },
            [](BoxCollider& self, Vector2& point) { return IsColliding(&point, &self); }));

    lua.new_usertype<Button>("Button", sol::base_classes, sol::bases<Component>(),
        "addFunction", &Button::addFunction, "zOrder", &Button::zOrder);

    lua.new_usertype<Sprite>("Sprite", sol::base_classes, sol::bases<Component>(),
        "zIndex", &Sprite::z_index,
        "hasTexture", sol::readonly(&Sprite::hasTexture),
        "color", &Sprite::color,
        "path",
        sol::property(
            [](Sprite* sprite) -> std::string& { return sprite->path; },
            [](Sprite* sprite, const std::string& value) { sprite->SetPath(value); }
        ),
        "reload", &Sprite::Reload,
        "size", &Sprite::size,
        "srcRect",
        sol::property(
            [](Sprite* sprite) -> SDL_Rect& { return sprite->srcRect; },
            [](Sprite* sprite, const SDL_Rect& value) { sprite->srcRect = value; }));

    lua.new_usertype<Text>("Text", sol::base_classes, sol::bases<Component>(),
        "text",
        sol::property(
            [](Text& self) -> std::string& { return self.text; },
            [](Text& self, const std::string& value) {
                self.text = value;
                self.Reload();
            }
        ),
        "reload", &Text::Reload,
        "fontName",
        sol::property(
            [](Text& self) -> std::string& { return self.fontName; },
            [](Text& self, const std::string& value) {
                self.fontName = value;
                self.Reload();
            }
        ),
        "fontSize",
        sol::property(
            [](Text& self) -> int& { return self.fontSize; },
            [](Text& self, int value) {
                self.fontSize = value;
                self.Reload();
            }
        ),
        "color",
        sol::property(
            [](Text& self) -> SDL_Color& { return self.color; },
            [](Text& self, const SDL_Color& value) {
                self.color = value;
                self.Reload();
            }
        )
    );

    lua.new_usertype<Animator>("Animator", sol::base_classes, sol::bases<Component>(),
        "Play", &Animator::Play,
        "Pause", &Animator::Pause,
        "Resume", &Animator::Resume,
        "Stop", &Animator::Stop,
        "SetSpeed", &Animator::SetSpeed,
        "playing",
        sol::property(
            [](Animator& self) -> bool { return self.playing; },
            [](Animator& self, bool value) {
                if (value) self.Resume();
                else self.Pause();
            }
        ),
        "speed",
        sol::property(
            [](Animator& self) -> float { return self.speed; },
            [](Animator& self, float value) { self.SetSpeed(value); }
        )
    );

    lua.new_usertype<Rigidbody>("Rigidbody", sol::base_classes, sol::bases<Component>(),
        "bodyType", sol::property(
            [](Rigidbody& self) -> int {
                return static_cast<int>(self.bodyType);
            },
            [](Rigidbody& self, int type) {
                if (type >= 0 && type <= 2) {
                    self.setBodyType(static_cast<BodyType>(type));
                }
            }
        ),
        "density", &Rigidbody::density,
        "friction", &Rigidbody::friction,
        "velocity",
        sol::property(
            [](Rigidbody& self) -> Vector2 { return self.velocity; },
            [](Rigidbody& self, const Vector2& value) { self.setVelocity(value); }
        ),
        "applyImpulse", &Rigidbody::applyImpulse,
        "setVelocity", &Rigidbody::setVelocity,
        "setDensity", &Rigidbody::setDensity,
        "setFriction", &Rigidbody::setFriction,
        "setBodyType", &Rigidbody::setBodyType
    );
    
    lua.new_usertype<GameObjectHandle>(
        "GameObject",
        sol::no_constructor,
        "valid",
        sol::property(
            [](GameObjectHandle& self) -> bool {
                return self.isValid();
            }
        ),
        "isValid",
        [](GameObjectHandle& self) -> bool {
            return self.isValid();
        },
        "id",
        sol::property(
            [](GameObjectHandle& self) -> sol::object {
                GameObject* go = self.resolveOrLog();
                if (!go)
                    return sol::nil;
                return sol::make_object(::lua, go->id);
            }
        ),
        "name",
        sol::property(
            [](GameObjectHandle& self) -> sol::object {
                GameObject* go = self.resolveOrLog();
                if (!go)
                    return sol::nil;
                return sol::make_object(::lua, go->name);
            },
            [](GameObjectHandle& self, const std::string& value) {
                GameObject* go = self.resolveOrLog();
                if (!go)
                    return;
                go->name = value;
            }
        ),
        "tag",
        sol::property(
            [](GameObjectHandle& self) -> sol::object {
                GameObject* go = self.resolveOrLog();
                if (!go)
                    return sol::nil;
                return sol::make_object(::lua, go->tag);
            },
            [](GameObjectHandle& self, const std::string& value) {
                GameObject* go = self.resolveOrLog();
                if (!go)
                    return;
                go->tag = value;
            }
        ),
        "transform",
        sol::property(
            [](GameObjectHandle& self) -> sol::object {
                GameObject* go = self.resolveOrLog();
                if (!go)
                    return sol::nil;
                return sol::make_object(::lua, &go->transform);
            },
            [](GameObjectHandle& self, const Transform& value) {
                GameObject* go = self.resolveOrLog();
                if (!go)
                    return;
                go->transform = value;
            }
        ),
        "onCollisionEnter",
        sol::property(
            [](GameObjectHandle& self) -> sol::object {
                GameObject* go = self.resolveOrLog();
                if (!go || !go->onCollisionEnterCallback.valid())
                    return sol::nil;
                return go->onCollisionEnterCallback;
            },
            [](GameObjectHandle& self, const sol::function& callback) {
                GameObject* go = self.resolveOrLog();
                if (!go)
                    return;
                go->onCollisionEnterCallback = callback;
            }
        ),
        "onCollisionExit",
        sol::property(
            [](GameObjectHandle& self) -> sol::object {
                GameObject* go = self.resolveOrLog();
                if (!go || !go->onCollisionExitCallback.valid())
                    return sol::nil;
                return go->onCollisionExitCallback;
            },
            [](GameObjectHandle& self, const sol::function& callback) {
                GameObject* go = self.resolveOrLog();
                if (!go)
                    return;
                go->onCollisionExitCallback = callback;
            }
        ),
        "onTriggerEnter",
        sol::property(
            [](GameObjectHandle& self) -> sol::object {
                GameObject* go = self.resolveOrLog();
                if (!go || !go->onTriggerEnterCallback.valid())
                    return sol::nil;
                return go->onTriggerEnterCallback;
            },
            [](GameObjectHandle& self, const sol::function& callback) {
                GameObject* go = self.resolveOrLog();
                if (!go)
                    return;
                go->onTriggerEnterCallback = callback;
            }
        ),
        "onTriggerExit",
        sol::property(
            [](GameObjectHandle& self) -> sol::object {
                GameObject* go = self.resolveOrLog();
                if (!go || !go->onTriggerExitCallback.valid())
                    return sol::nil;
                return go->onTriggerExitCallback;
            },
            [](GameObjectHandle& self, const sol::function& callback) {
                GameObject* go = self.resolveOrLog();
                if (!go)
                    return;
                go->onTriggerExitCallback = callback;
            }
        ),

        "addComponent",
        [](GameObjectHandle& self, const std::string& componentName)
            -> sol::object {
            GameObject* go = self.resolveOrLog();

            if (!go)
                return sol::nil;

            return LuaApi::addComponent(*go, componentName);
        },

        "getComponent",
        [](GameObjectHandle& self, const std::string& componentName)
            -> sol::object {
            GameObject* go = self.resolveOrLog();

            if (!go)
                return sol::nil;

            return LuaApi::getComponent(*go, componentName);
        },

        "destroy",
        [](GameObjectHandle& self) {
            if (GameObject* go = self.resolve())
                SceneManager::getInstance().deleteObjectById(go->id);
        },
        sol::meta_function::equal_to,
        [](const GameObjectHandle& a, const GameObjectHandle& b) {
            return a.id == b.id;
        },
        sol::meta_function::to_string,
        [](const GameObjectHandle& self) {
            return std::string("GameObject(id=") +
                   std::to_string(self.id) +
                   ", valid=" +
                   (self.isValid() ? "true" : "false") +
                   ")";
        }
    );

    lua.new_usertype<Audio>("Audio", sol::base_classes, sol::bases<Component>(),
        "name", &Audio::name,
        "spatial", &Audio::spatial,
        "maxDistance", &Audio::maxDistance,
        "channel", &Audio::channel,
        "loops", &Audio::loops,
        "volume",
        sol::property(
            [](Audio& self) -> int { return self.GetVolume(); },
            [](Audio& self, int value) { self.SetVolume(value); }
        ),
        "Play", &Audio::Play,
        "Stop", &Audio::Stop);

    lua.new_usertype<RaycastHit>(
        "RaycastHit",
        sol::no_constructor,
        "hit", sol::readonly(&RaycastHit::hit),
        "point", sol::readonly(&RaycastHit::point),
        "normal", sol::readonly(&RaycastHit::normal),
        "fraction", sol::readonly(&RaycastHit::fraction)
    );

    lua["Physics"] = lua.create_table();
    lua["Physics"]["raycast"] = [](Vector2 start, Vector2 end) -> RaycastHit {
        if (!physics.has_value()) {
            throw sol::error("Physics world is not initialized");
        }
        return physics->raycast(start, end);
    };
    lua["Physics"]["setGravity"] = [](Vector2 gravity) {
        if (!physics.has_value()) {
            throw sol::error("Physics world is not initialized");
        }
        physics->setGravity(gravity);
    };
}