#include "LuaBindings.h"
#include "iostream"
#include "LuaApi.h"
#include "component/Animator.h"
#include "component/Rigidbody.h"
#include "component/Text.h"
#include "core/Log.h"
#include "core/State.h"
#include "render/TextureManager.h"
#include "scene/GameObject.h"
#include "utils/FileSystem.h"

sol::state lua;
std::vector<LuaObject> scripts;


void LuaBindings::bindCore(sol::state& lua) {
    lua.open_libraries(
        sol::lib::base,
        sol::lib::math,
        sol::lib::table,
        sol::lib::string
    );
}

void LuaBindings::bindMath(sol::state& lua) {
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
        SceneManager::getInstance().loadScene(name);
        Lua::loadSceneScripts(name);
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
}

void LuaBindings::bindECS(sol::state& lua) {
    lua.new_usertype<Component>("Component",
        "Play", [](Component* c, const std::string& name){
            if(auto a = dynamic_cast<Animator*>(c)) a->Play(name);
        },
        "Pause", [](Component* c){
            if(auto a = dynamic_cast<Animator*>(c)) a->Pause();
        },
        "Resume", [](Component* c){
            if(auto a = dynamic_cast<Animator*>(c)) a->Resume();
        },
        "Stop", [](Component* c){
            if(auto a = dynamic_cast<Animator*>(c)) a->Stop();
        },
        "SetSpeed", [](Component* c, float s){
            if(auto a = dynamic_cast<Animator*>(c)) a->SetSpeed(s);
        },
        "zIndex", sol::property(
            [](Component* c) -> int {
                if (auto s = dynamic_cast<Sprite*>(c))
                    return s->z_index;
                return 0;
            },
            [](Component* c, int v) {
                if (auto s = dynamic_cast<Sprite*>(c))
                    s->z_index = v;
            }
        ),
        "path", sol::property(
            [](Component* c) -> std::string& {
                if (const auto s = dynamic_cast<Sprite*>(c))
                    return s->path;
                throw std::invalid_argument("Not a Sprite");
            },
            [](Component* c, const std::string& v) {
                if (const auto s = dynamic_cast<Sprite*>(c))
                    s->path = v;
            }
        ),
        "velocity", sol::property(
            [](Component* c) -> Vector2& {
                if (const auto rb = dynamic_cast<Rigidbody*>(c))
                    return rb->velocity;
                throw std::runtime_error("Not a Rigidbody");
            }
        ),
        "reload", [](Component* c) {
            if (const auto sp = dynamic_cast<Sprite*>(c)) sp->Reload();
            if (const auto tx = dynamic_cast<Text*>(c)) tx->Reload();
        },
        "size", [](Component* c) -> Vector2& {
            if (const auto sp = dynamic_cast<Sprite*>(c)) {
                return sp->size();
            }
            if (const auto tx = dynamic_cast<Text*>(c)) {
                return tx->size();
            }
            gameLog("Not a Sprite or Text", ERROR);
            throw std::runtime_error("Not a Sprite or Text");
        },
        "text", sol::property(
            [](Component* c) -> std::string& {
                if (const auto tx = dynamic_cast<Text*>(c)) {
                    return tx->text;
                }
                gameLog("Not a Text", ERROR);
                throw std::runtime_error("Not a Text");
            },
            [](Component* c, const std::string& v) {
                if (const auto tx = dynamic_cast<Text*>(c)) {
                    tx->text = v;
                }
            }
        ),
        "fontName", sol::property(
            [](Component* c) -> std::string& {
                if (const auto tx = dynamic_cast<Text*>(c)) {
                    return tx->fontName;
                }
                gameLog("Not a Text", ERROR);
                throw std::runtime_error("Not a Text");
            },
            [](Component* c, const std::string& v) {
                if (const auto tx = dynamic_cast<Text*>(c)) {
                    tx->fontName = v;
                }
            }
        ),
        "fontSize", sol::property(
            [](Component* c) -> int& {
                if (const auto tx = dynamic_cast<Text*>(c)) {
                    return tx->fontSize;
                }
                gameLog("Not a Text", ERROR);
                throw std::runtime_error("Not a Text");
            },
            [](Component* c, int v) {
                if (const auto tx = dynamic_cast<Text*>(c)) {
                    tx->fontSize = v;
                }
            }
        ),
        "color", sol::property(
            [](Component* c) -> SDL_Color& {
                if (const auto tx = dynamic_cast<Text*>(c)) {
                    return tx->color;
                }
                gameLog("Not a Text", ERROR);
                throw std::runtime_error("Not a Text");
            },
            [](Component* c, const SDL_Color& v) {
                if (const auto tx = dynamic_cast<Text*>(c)) {
                    tx->color = v;
                }
            })
    );

    lua.new_usertype<Transform>("Transform",
        "position", &Transform::position,
        "scale", &Transform::scale
    );

    lua.new_usertype<Sprite>("Sprite",
        "z_index", &Sprite::z_index,
        "path", &Sprite::path,
        "reload", &Sprite::Reload,
        "size", &Sprite::size
    );

    lua.new_usertype<Text>("Text",
        "text", &Text::text,
        "reload", &Text::Reload,
        "fontName", &Text::fontName,
        "fontSize", &Text::fontSize,
        "color", &Text::color
    );

    lua.new_usertype<Animator>("Animator",
        "Play", &Animator::Play,
        "Pause", &Animator::Pause,
        "Resume", &Animator::Resume,
        "Stop", &Animator::Stop,
        "SetSpeed", &Animator::SetSpeed
    );

    lua.new_usertype<Rigidbody>("Rigidbody",
        "velocity", &Rigidbody::velocity
    );

    lua.new_usertype<GameObject>(
        "GameObject",
        "id", &GameObject::id,
        "name", &GameObject::name,
        "transform", &GameObject::transform,
        "addComponent", &LuaApi::addComponent,
        "getComponent", &LuaApi::getComponent
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

    mouse.set_function("position", &LuaApi::getMousePosition);
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
    LuaBindings::bindScene(lua);
    LuaBindings::bindInput(lua);
    LuaBindings::bindDebug(lua);
    LuaBindings::bindAsset(lua);
    LuaBindings::bindState(lua);
    LuaBindings::bindECS(lua);

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

    callStartLua();
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