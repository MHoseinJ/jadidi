#include "component/Factory.h"
#include "Audio.h"
#include "Button.h"
#include "Collider.h"
#include "LuaComponent.h"
#include "Rigidbody.h"
#include "Text.h"

void Factory::registerComponent(std::string_view name, Creator fn) {
    creators.emplace(name, std::move(fn));
}

void registerComponents() {
    auto& f = Factory::instance();
    f.registerComponent("transform", [] { return std::make_unique<Transform>(); });
    f.registerComponent("sprite", [] { return std::make_unique<Sprite>(); });
    f.registerComponent("animator", [] { return std::make_unique<Animator>(); });
    f.registerComponent("rigidbody", [] { return std::make_unique<Rigidbody>(); });
    f.registerComponent("text", [] { return std::make_unique<Text>(); });
    f.registerComponent("boxCollider", [] { return std::make_unique<BoxCollider>(); });
    f.registerComponent("button", [] { return std::make_unique<Button>(); });
    f.registerComponent("audio", [] { return std::make_unique<Audio>(); });
}

std::unique_ptr<Component> Factory::create(const std::string& name) const {
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
        throw std::runtime_error("Unknown component '" + name + "'. Did you mean '" + suggestion + "'?");
    }
    throw std::runtime_error("Unknown component '" + name + "'");
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
                curr[j - 1] + 1,   // insert
                prev[j] + 1,       // delete
                prev[j - 1] + cost // replace
            });
        }
        std::swap(prev, curr);
    }
    return prev[b.size()];
}