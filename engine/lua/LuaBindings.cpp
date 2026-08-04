#include "LuaBindings.h"
#include "component/Component.h"
#include "component/Sprite.h"
#include "core/Input.h"
#include "iostream"
#include "LuaApi.h"
#include "component/Animator.h"
#include "component/Audio.h"
#include "component/Button.h"
#include "component/Collider.h"
#include "component/Rigidbody.h"
#include "component/Text.h"
#include "core/Audio.h"
#include "core/Log.h"
#include "core/State.h"
#include "render/TextureManager.h"
#include "scene/GameObject.h"
#include "utils/FileSystem.h"
#include <sol/error.hpp>

sol::state lua;
std::vector<LuaObject> scripts;
static std::string pendingSceneLoad = "";

void LuaBindings::bindCore(sol::state& lua) {
    lua.open_libraries(
        sol::lib::base,
        sol::lib::math,
        sol::lib::table,
        sol::lib::string
    );
}

void LuaBindings::bindMath(sol::state& lua) {
    lua.new_usertype<SDL_Rect>(
        "SDL_Rect",
        "x", &SDL_Rect::x,
        "y", &SDL_Rect::y,
        "w", &SDL_Rect::w,
        "h", &SDL_Rect::h
    );
    lua.new_usertype<Vector2>(
        "Vector2",
        sol::constructors<Vector2(), Vector2(float, float)>(),
        "x", &Vector2::x,
        "y", &Vector2::y,
        "set", &Vector2::set,
        "move", &Vector2::move
    );
    lua.new_usertype<SDL_Color>(
        "Color",
        sol::constructors<SDL_Color()>(),
        "r", &SDL_Color::r,
        "g", &SDL_Color::g,
        "b", &SDL_Color::b,
        "a", &SDL_Color::a
    );
}

void LuaBindings::bindScene(sol::state& lua) {

    // bind screen
    auto screen = lua["Screen"].get_or_create<sol::table>();
    screen.set_function("size", &LuaApi::getScreenSize);

    auto scene = lua["Scene"].get_or_create<sol::table>();

    // bind loadScene
    scene.set_function("load", [](const std::string& name) {
        pendingSceneLoad = name;
    });

    auto gameobject = lua["Objects"].get_or_create<sol::table>();

    // find overloads
    gameobject.set_function("find",
        sol::overload(
            [](const std::string& name) -> GameObject* {
                return SceneManager::getInstance().findGameObjectWithName(name);
            },
            [](int id) -> GameObject* {
                return SceneManager::getInstance().findGameObjectWithId(id);
            }
        )
    );

    gameobject.set_function("create", [](const std::string& name) -> GameObject& {
        return SceneManager::getInstance().createObject(name);
    });

    gameobject.set_function("deleteById", [](int id){
        SceneManager::getInstance().deleteObjectById(id);
    });
    gameobject.set_function("deleteByName", [](const std::string& name){
        SceneManager::getInstance().deleteAllObjectsByName(name);
    });
    gameobject.set_function("deleteByTag", [](const std::string& tag){
        SceneManager::getInstance().deleteAllObjectsByTag(tag);
    });

    lua.new_usertype<Camera>("Camera",
        "transform", &Camera::transform,
        "zoom", &Camera::zoom
    );

    lua["Camera"] = &camera;

    auto audio_system = lua["AudioSystem"].get_or_create<sol::table>();

    audio_system.set_function("load", [](const std::string& name, const std::string& path, const bool isMusic) {
        gameLog("Lua calling load: name='" + name + "', path='" + path + "', isMusic=" + std::to_string(isMusic), INFO);
        AudioSystem::getInstance().LoadSound(name, path, isMusic);
    });

}

void LuaBindings::bindECS(sol::state& lua) {

    lua.new_usertype<Component>(
        "Component",

        // ------------------------------------------------------------
        // Animator / Audio
        // ------------------------------------------------------------

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

        // ------------------------------------------------------------
        // Sprite
        // ------------------------------------------------------------

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

        // ------------------------------------------------------------
        // Rigidbody
        // ------------------------------------------------------------

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

        // ------------------------------------------------------------
        // Size
        // ------------------------------------------------------------

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

        // ------------------------------------------------------------
        // Text
        // ------------------------------------------------------------

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

        // ------------------------------------------------------------
        // Button
        // ------------------------------------------------------------

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
            }
        ),

        // ------------------------------------------------------------
        // Audio
        // ------------------------------------------------------------

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

        // ------------------------------------------------------------
        // Collision
        // ------------------------------------------------------------

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


    // ========================================================================
    // Transform
    // ========================================================================

    lua.new_usertype<Transform>(
        "Transform",
        "position", &Transform::position,
        "scale", &Transform::scale
    );


    // ========================================================================
    // BoxCollider
    // ========================================================================

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


    // ========================================================================
    // Button
    // ========================================================================

    lua.new_usertype<Button>(
        "Button",
        sol::base_classes,
        sol::bases<Component>(),

        "addFunction", &Button::addFunction,
        "zOrder", &Button::zOrder
    );


    // ========================================================================
    // Sprite
    // ========================================================================

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


    // ========================================================================
    // Text
    // ========================================================================

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


    // ========================================================================
    // Animator
    // ========================================================================

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


    // ========================================================================
    // Rigidbody
    // ========================================================================

    lua.new_usertype<Rigidbody>(
        "Rigidbody",
        sol::base_classes,
        sol::bases<Component>(),

        "velocity", &Rigidbody::velocity
    );


    // ========================================================================
    // GameObject
    // ========================================================================

    lua.new_usertype<GameObject>(
        "GameObject",

        "id", &GameObject::id,
        "name", &GameObject::name,
        "transform", &GameObject::transform,

        "addComponent", &LuaApi::addComponent,
        "getComponent", &LuaApi::getComponent
    );


    // ========================================================================
    // Audio
    // ========================================================================

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

void LuaBindings::bindState(sol::state &lua) {
    auto state = lua["State"].get_or_create<sol::table>();

    state.set_function("set", [](const std::string& key, const sol::object &value) {
        State::instance().set(key, value);
    });

    state.set_function("get", [](const std::string& key) -> sol::object {
        return State::instance().get(key);
    });

    state.set_function("exists", [](const std::string& key) {
        return State::instance().exists(key);
    });

    state.set_function("remove", [](const std::string& key) {
        State::instance().remove(key);
    });

    state.set_function("clear", []() {
        State::instance().clear();
    });
}

void LuaBindings::bindJson(sol::state& lua) {
    auto json = lua["Json"].get_or_create<sol::table>();

    json.set_function("read", [&](const std::string& path) -> sol::object {
        if (!fs::fileExists(path))
            return sol::nil;

        auto j = fs::readJson(path);
        return LuaApi::LuaJSON(j);
    });

    json.set_function("write", [&](const std::string& path, const sol::object& table) -> bool {
        if (table.get_type() != sol::type::table)
            return false;

        auto j = LuaApi::LuaJSON(table);
        return fs::writeJson(path, j);
    });
}

void LuaBindings::bindInput(sol::state& lua) {
    LuaApi::bindKeys(lua);
    LuaApi::bindMouse(lua);

    auto input = lua["Input"].get_or_create<sol::table>();

    input.set_function("keyPressed",
        static_cast<bool(*)(int)>(&LuaApi::isKeyPressed)
    );

    input.set_function("keyDown",
        static_cast<bool(*)(int)>(&LuaApi::isKeyDown)
    );

    input.set_function("keyUp",
        static_cast<bool(*)(int)>(&LuaApi::isKeyUp)
    );

    input.set_function("mousePressed",
        static_cast<bool(*)(int)>(&LuaApi::isMousePressed)
    );
    input.set_function("mouseDown",
        static_cast<bool(*)(int)>(&LuaApi::isMouseDown)
    );
    input.set_function("mouseUp",
        static_cast<bool(*)(int)>(&LuaApi::isMouseUp)
    );

    auto mouse = lua["Mouse"].get_or_create<sol::table>();

    mouse.set_function("position", &Input::GetMousePositionVec);
    mouse.set_function("world_position", &Input::GetMouseWorldPos);
}

void LuaBindings::bindDebug(sol::state& lua) {
    auto log = lua["Log"].get_or_create<sol::table>();

    log.set_function("clear", &LuaApi::clear);
    log.set_function("print", &LuaApi::print);
    log.set_function("info",  &LuaApi::info);
    log.set_function("warn",  &LuaApi::warn);
    log.set_function("error", &LuaApi::error);
}

void LuaBindings::bindAsset(sol::state& lua) {
    // TODO: bind asset system
}

void Lua::loadSceneScripts(const std::string& sceneName) {
    scripts.clear();
    lua.collect_garbage();

    LuaBindings::bindCore(lua);
    LuaBindings::bindMath(lua);
    LuaBindings::bindInput(lua);
    LuaBindings::bindDebug(lua);
    LuaBindings::bindAsset(lua);
    LuaBindings::bindState(lua);
    LuaBindings::bindECS(lua);
    LuaBindings::bindScene(lua);

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

        auto target = env["RUN_IN_SCENE"];
        if (!target.valid() || target.get<std::string>() != sceneName)
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

    LuaObject& obj = scripts[0];

    for (auto& pair : obj.env) {
        sol::object key = pair.first;
        sol::object value = pair.second;

        std::string keyStr = key.as<std::string>();
        std::string typeStr = (value.get_type() == sol::type::function) ? "function" : "value";
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
    if (!pendingSceneLoad.empty()) {
        SceneManager::getInstance().loadScene(pendingSceneLoad);
        Lua::loadSceneScripts(pendingSceneLoad);
        callStartLua();
        pendingSceneLoad = "";
    }
}