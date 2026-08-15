#include "lua/LuaBindings.h"
#include "lua/GameObjectHandle.h"
#include "component/Component.h"
#include "component/Sprite.h"
#include "component/Animator.h"
#include "component/Audio.h"
#include "component/Button.h"
#include "component/Collider.h"
#include "component/Rigidbody.h"
#include "component/Text.h"
#include "core/Engine.h"
#include "lua/LuaApi.h"
#include <sol/error.hpp>

void LuaBindings::bindECS(sol::state& lua) {

    lua.new_usertype<Component>(
        "Component",

        "Play",
        [](Component* c, const std::string& name, int loops = 0) {
            if (auto* animator = dynamic_cast<Animator*>(c)) {
                animator->Play(name);
                return;
            }

            if (auto* audio = dynamic_cast<Audio*>(c)) {
                audio->Play(name, loops);
                return;
            }

            throw sol::error(
                "Play() requires an Animator or Audio component"
            );
        },

        "Pause",
        [](Component* c) {
            if (auto* animator = dynamic_cast<Animator*>(c)) {
                animator->Pause();
                return;
            }

            throw sol::error(
                "Pause() requires an Animator component"
            );
        },

        "Resume",
        [](Component* c) {
            if (auto* animator = dynamic_cast<Animator*>(c)) {
                animator->Resume();
                return;
            }

            throw sol::error(
                "Resume() requires an Animator component"
            );
        },

        "Stop",
        [](Component* c) {
            if (auto* animator = dynamic_cast<Animator*>(c)) {
                animator->Stop();
                return;
            }

            if (auto* audio = dynamic_cast<Audio*>(c)) {
                audio->Stop();
                return;
            }

            throw sol::error(
                "Stop() requires an Animator or Audio component"
            );
        },

        "SetSpeed",
        [](Component* c, float speed) {
            if (auto* animator = dynamic_cast<Animator*>(c)) {
                animator->SetSpeed(speed);
                return;
            }

            throw sol::error(
                "SetSpeed() requires an Animator component"
            );
        },

        "zIndex",
        sol::property(
            [](Component* c) -> int {
                if (auto* sprite = dynamic_cast<Sprite*>(c)) {
                    return sprite->z_index;
                }

                throw sol::error(
                    "zIndex is only available on Sprite components"
                );
            },

            [](Component* c, int value) {
                if (auto* sprite = dynamic_cast<Sprite*>(c)) {
                    sprite->z_index = value;
                    rendererInterface->markDirty();
                    return;
                }

                throw sol::error(
                    "zIndex is only available on Sprite components"
                );
            }
        ),

        "path", sol::property(
            [](Component* c) -> std::string& {
                if (auto* s = dynamic_cast<Sprite*>(c)) {
                    return s->path;
                }

                throw sol::error(
                    "path is only available on Sprite components"
                );
            },

            [](Component* c, const std::string& value) {
                if (auto* s = dynamic_cast<Sprite*>(c)) {
                    s->SetPath(value);
                    return;
                }

                throw sol::error(
                    "path is only available on Sprite components"
                );
            }
        ),

        "reload",
        [](Component* c) {
            if (auto* sprite = dynamic_cast<Sprite*>(c)) {
                sprite->Reload();
                return;
            }

            if (auto* text = dynamic_cast<Text*>(c)) {
                text->Reload();
                return;
            }

            throw sol::error(
                "reload() requires a Sprite or Text component"
            );
        },

        "srcRect",
        sol::property(
            [](Component* c) -> SDL_Rect& {
                if (auto* sprite = dynamic_cast<Sprite*>(c)) {
                    return sprite->srcRect;
                }

                throw sol::error(
                    "srcRect is only available on Sprite components"
                );
            },

            [](Component* c, const SDL_Rect& value) {
                if (auto* sprite = dynamic_cast<Sprite*>(c)) {
                    sprite->srcRect = value;
                    return;
                }

                throw sol::error(
                    "srcRect is only available on Sprite components"
                );
            }
        ),

        "velocity",
        sol::property(
            [](Component* c) -> Vector2& {
                if (auto* rigidbody = dynamic_cast<Rigidbody*>(c)) {
                    return rigidbody->velocity;
                }

                throw sol::error(
                    "velocity is only available on Rigidbody components"
                );
            }
        ),

        "size",
        sol::property(
            [](Component* c) -> Vector2& {
                if (auto* sprite = dynamic_cast<Sprite*>(c)) {
                    return sprite->size();
                }

                if (auto* text = dynamic_cast<Text*>(c)) {
                    return text->size();
                }

                if (auto* collider = dynamic_cast<BoxCollider*>(c)) {
                    return collider->size;
                }

                throw sol::error(
                    "this component does not have a size property"
                );
            },

            [](Component* c, const Vector2& value) {
                if (auto* collider = dynamic_cast<BoxCollider*>(c)) {
                    collider->size = value;
                    return;
                }

                throw sol::error(
                    "size can only be assigned to BoxCollider components"
                );
            }
        ),

        "text",
        sol::property(
            [](Component* c) -> std::string& {
                if (auto* text = dynamic_cast<Text*>(c)) {
                    return text->text;
                }

                throw sol::error(
                    "text is only available on Text components"
                );
            },

            [](Component* c, const std::string& value) {
                if (auto* text = dynamic_cast<Text*>(c)) {
                    text->text = value;
                    return;
                }

                throw sol::error(
                    "text is only available on Text components"
                );
            }
        ),

        "fontName",
        sol::property(
            [](Component* c) -> std::string& {
                if (auto* text = dynamic_cast<Text*>(c)) {
                    return text->fontName;
                }

                throw sol::error(
                    "fontName is only available on Text components"
                );
            },

            [](Component* c, const std::string& value) {
                if (auto* text = dynamic_cast<Text*>(c)) {
                    text->fontName = value;
                    return;
                }

                throw sol::error(
                    "fontName is only available on Text components"
                );
            }
        ),

        "fontSize",
        sol::property(
            [](Component* c) -> int& {
                if (auto* text = dynamic_cast<Text*>(c)) {
                    return text->fontSize;
                }

                throw sol::error(
                    "fontSize is only available on Text components"
                );
            },

            [](Component* c, int value) {
                if (auto* text = dynamic_cast<Text*>(c)) {
                    text->fontSize = value;
                    return;
                }

                throw sol::error(
                    "fontSize is only available on Text components"
                );
            }
        ),

        "color",
        sol::property(
            [](Component* c) -> SDL_Color& {
                if (auto* text = dynamic_cast<Text*>(c)) {
                    return text->color;
                }

                throw sol::error(
                    "color is only available on Text components"
                );
            },

            [](Component* c, const SDL_Color& value) {
                if (auto* text = dynamic_cast<Text*>(c)) {
                    text->color = value;
                    return;
                }

                throw sol::error(
                    "color is only available on Text components"
                );
            }
        ),

        "addFunction",
        [](Component* c, const sol::function& function, int value) {
            if (auto* button = dynamic_cast<Button*>(c)) {
                button->addFunction(function, value);
                return;
            }

            throw sol::error(
                "addFunction() is only available on Button components"
            );
        },

        "zOrder",
        sol::property(
            [](Component* c) -> int& {
                if (auto* button = dynamic_cast<Button*>(c)) {
                    return button->zOrder;
                }

                throw sol::error(
                    "zOrder is only available on Button components"
                );
            },
            [](Component* c, int value) {
                if (auto* button = dynamic_cast<Button*>(c)) {
                    button->zOrder = value;
                    return;
                }
                throw sol::error(
                    "zOrder is only available on Button components"
                );
            }
        ),

        "name",
        sol::property(
            [](Component* c) -> std::string& {
                if (auto* audio = dynamic_cast<Audio*>(c)) {
                    return audio->name;
                }

                throw sol::error(
                    "name is only available on Audio components"
                );
            },

            [](Component* c, const std::string& value) {
                if (auto* audio = dynamic_cast<Audio*>(c)) {
                    audio->name = value;
                    return;
                }

                throw sol::error(
                    "name is only available on Audio components"
                );
            }
        ),

        "spatial",
        sol::property(
            [](Component* c) -> bool& {
                if (auto* audio = dynamic_cast<Audio*>(c)) {
                    return audio->spatial;
                }

                throw sol::error(
                    "spatial is only available on Audio components"
                );
            },

            [](Component* c, bool value) {
                if (auto* audio = dynamic_cast<Audio*>(c)) {
                    audio->spatial = value;
                    return;
                }

                throw sol::error(
                    "spatial is only available on Audio components"
                );
            }
        ),

        "maxDistance",
        sol::property(
            [](Component* c) -> float& {
                if (auto* audio = dynamic_cast<Audio*>(c)) {
                    return audio->maxDistance;
                }

                throw sol::error(
                    "maxDistance is only available on Audio components"
                );
            },

            [](Component* c, float value) {
                if (auto* audio = dynamic_cast<Audio*>(c)) {
                    audio->maxDistance = value;
                    return;
                }

                throw sol::error(
                    "maxDistance is only available on Audio components"
                );
            }
        ),

        "volume",
        sol::property(
            [](Component* c) -> int {
                if (auto* audio = dynamic_cast<Audio*>(c)) {
                    return audio->GetVolume();
                }

                throw sol::error(
                    "volume is only available on Audio components"
                );
            },

            [](Component* c, int value) {
                if (auto* audio = dynamic_cast<Audio*>(c)) {
                    audio->SetVolume(value);
                    return;
                }

                throw sol::error(
                    "volume is only available on Audio components"
                );
            }
        ),

        "loops",
        sol::property(
            [](Component* c) -> int& {
                if (auto* audio = dynamic_cast<Audio*>(c)) {
                    return audio->loops;
                }

                throw sol::error(
                    "loops is only available on Audio components"
                );
            },

            [](Component* c, int value) {
                if (auto* audio = dynamic_cast<Audio*>(c)) {
                    audio->loops = value;
                    return;
                }

                throw sol::error(
                    "loops is only available on Audio components"
                );
            }
        ),

        "overlap",
        sol::overload(

            [](Component* c, Component* other) {
                auto* a = dynamic_cast<BoxCollider*>(c);
                auto* b = dynamic_cast<BoxCollider*>(other);

                if (!a || !b) {
                    throw sol::error(
                        "overlap() requires two BoxCollider components"
                    );
                }

                return IsColliding(a, b);
            },

            [](Component* c, const Vector2& point) {
                auto* box = dynamic_cast<BoxCollider*>(c);

                if (!box) {
                    throw sol::error(
                        "overlap() requires a BoxCollider component"
                    );
                }

                return IsColliding(&point, box);
            }
        )
    );

    lua.new_usertype<Transform>(
        "Transform",
        "position",
        sol::property(
            [](Transform& self) -> Vector2& { return self.position; },
            [](Transform& self, const Vector2& value) { self.position = value; }
        ),
        "scale",
        sol::property(
            [](Transform& self) -> Vector2& { return self.scale; },
            [](Transform& self, const Vector2& value) { self.scale = value; }
        )
    );

    lua.new_usertype<BoxCollider>(
        "BoxCollider",
        sol::base_classes,
        sol::bases<Component>(),

        "size", &BoxCollider::size,

        "overlap",
        sol::overload(

            [](BoxCollider& self, BoxCollider& other) {
                return IsColliding(&self, &other);
            },

            [](BoxCollider& self, Vector2& point) {
                return IsColliding(&point, &self);
            }
        )
    );

    lua.new_usertype<Button>(
        "Button",
        sol::base_classes,
        sol::bases<Component>(),

        "addFunction", &Button::addFunction,
        "zOrder", &Button::zOrder
    );

    lua.new_usertype<Sprite>(
        "Sprite",
        sol::base_classes,
        sol::bases<Component>(),

        "zIndex", &Sprite::z_index,

        "path", sol::property(
            [](Sprite* sprite) -> std::string& {
                return sprite->path;
            },
            [](Sprite* sprite, const std::string& value) {
                sprite->SetPath(value);
            }
        ),

        "reload", &Sprite::Reload,
        "size", &Sprite::size,

        "srcRect", sol::property(
            [](Sprite* sprite) -> SDL_Rect& {
                return sprite->srcRect;
            },
            [](Sprite* sprite, const SDL_Rect& value) {
                sprite->srcRect = value;
            }
        )
    );

    lua.new_usertype<Text>(
        "Text",
        sol::base_classes,
        sol::bases<Component>(),

        "text", &Text::text,
        "reload", &Text::Reload,
        "fontName", &Text::fontName,
        "fontSize", &Text::fontSize,
        "color", &Text::color
    );

    lua.new_usertype<Animator>(
        "Animator",
        sol::base_classes,
        sol::bases<Component>(),

        "Play", &Animator::Play,
        "Pause", &Animator::Pause,
        "Resume", &Animator::Resume,
        "Stop", &Animator::Stop,
        "SetSpeed", &Animator::SetSpeed
    );

    lua.new_usertype<Rigidbody>(
        "Rigidbody",
        sol::base_classes,
        sol::bases<Component>(),

        "velocity", &Rigidbody::velocity
    );

    lua.new_usertype<GameObjectHandle>(
        "GameObject",
        sol::no_constructor,
    
        "valid",
        sol::property(
            [](GameObjectHandle& self) {
                return self.isValid();
            }
        ),
    
        "isValid",
        [](GameObjectHandle& self) {
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
    
        "addComponent",
        [](GameObjectHandle& self, const std::string& componentName) -> sol::object {
            GameObject* go = self.resolveOrLog();
            if (!go)
                return sol::nil;
    
            Component* comp = LuaApi::addComponent(*go, componentName);
            if (!comp)
                return sol::nil;
    
            return sol::make_object(::lua, comp);
        },
    
        "getComponent",
        [](GameObjectHandle& self, const std::string& componentName) -> sol::object {
            GameObject* go = self.resolveOrLog();
            if (!go)
                return sol::nil;
    
            Component* comp = LuaApi::getComponent(*go, componentName);
            if (!comp)
                return sol::nil;
    
            return sol::make_object(::lua, comp);
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

    lua.new_usertype<Audio>(
        "Audio",
        sol::base_classes,
        sol::bases<Component>(),

        "name", &Audio::name,
        "spatial", &Audio::spatial,
        "volume", &Audio::volume,
        "maxDistance", &Audio::maxDistance,
        "channel", &Audio::channel,

        "Play", &Audio::Play,
        "Stop", &Audio::Stop
    );
}