
#include "component/Factory.h"

#include "Audio.h"
#include "Button.h"
#include "Collider.h"
#include "LuaComponent.h"
#include "Rigidbody.h"
#include "Text.h"

void Factory::registerComponent(
    std::string_view name,
    Creator fn,
    LuaPusher pusher
) {
    const std::string key(name);
    creators.emplace(key, std::move(fn));
    luaPushers.emplace(key, std::move(pusher));
}

void registerComponents() {
    auto& f = Factory::instance();

    f.registerComponent<Transform>(
        "transform",
        [] { return std::make_unique<Transform>(); }
    );

    f.registerComponent<Sprite>(
        "sprite",
        [] { return std::make_unique<Sprite>(); }
    );

    f.registerComponent<Animator>(
        "animator",
        [] { return std::make_unique<Animator>(); }
    );

    f.registerComponent<Rigidbody>(
        "rigidbody",
        [] { return std::make_unique<Rigidbody>(); }
    );

    f.registerComponent<Text>(
        "text",
        [] { return std::make_unique<Text>(); }
    );

    f.registerComponent<BoxCollider>(
        "boxCollider",
        [] { return std::make_unique<BoxCollider>(); }
    );

    f.registerComponent<Button>(
        "button",
        [] { return std::make_unique<Button>(); }
    );

    f.registerComponent<Audio>(
        "audio",
        [] { return std::make_unique<Audio>(); }
    );
}

std::unique_ptr<Component> Factory::create(
    const std::string& name
) const {
    if (name.rfind("lua:", 0) == 0) {
        auto comp = std::make_unique<LuaComponent>();
        comp->scriptPath = name.substr(4);
        return comp;
    }

    auto it = creators.find(name);

    if (it != creators.end())
        return it->second();

    std::string suggestion;
    int bestDistance = INT_MAX;

    for (const auto& [componentName, _] : creators) {
        int distance = levenshtein(name, componentName);

        if (distance < bestDistance) {
            bestDistance = distance;
            suggestion = componentName;
        }
    }

    if (bestDistance <= 2) {
        throw std::runtime_error(
            "Unknown component '" + name +
            "'. Did you mean '" + suggestion + "'?"
        );
    }

    throw std::runtime_error(
        "Unknown component '" + name + "'"
    );
}

sol::object Factory::toLua(
    sol::state_view lua,
    Component* component
) const {
    if (!component)
        return sol::nil;

    const std::string type = component->typeName();

    // Lua-scripted components are created dynamically by path.
    if (type.rfind("lua:", 0) == 0) {
        return sol::make_object(
            lua,
            static_cast<LuaComponent*>(component)
        );
    }

    auto it = luaPushers.find(type);

    if (it == luaPushers.end()) {
        throw std::runtime_error(
            "No Lua userdata converter registered for component '" +
            type + "'"
        );
    }

    return it->second(lua, component);
}

int levenshtein(const std::string& a, const std::string& b) {
    std::vector<int> prev(b.size() + 1);
    std::vector<int> curr(b.size() + 1);
    for (size_t j = 0; j <= b.size(); j++)
        prev[j] = j;
    for (size_t i = 1; i <= a.size(); i++) {
        curr[0] = i;
        for (size_t j = 1; j <= b.size(); j++) {
            int cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
            curr[j] = std::min({
                curr[j - 1] + 1,
                prev[j] + 1,
                prev[j - 1] + cost
            });
        }
        std::swap(prev, curr);
    }
    return prev[b.size()];
}