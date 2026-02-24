#include "component/Factory.h"

#include "Rigidbody.h"
//
// #include "component/Sprite.h"
// #include "component/Animator.h"
// #include "component/Transform.h"

// Factory& Factory::instance() {
//     static Factory f;
//     return f;
// }
//
void Factory::registerComponent(std::string_view name, Creator fn) {
    creators.emplace(name, std::move(fn));
}
//
// std::unique_ptr<Component> Factory::create(std::string_view name) const {
//     auto it = creators.find(std::string(name));
//     if (it == creators.end()) return nullptr;
//     return it->second();
// }
//
// // call once on engine init
// void registerComponents() {
//     auto& f = Factory::instance();
//     f.registerComponent("sprite", [] { return std::make_unique<Sprite>(); });
//     f.registerComponent("animator", [] { return std::make_unique<Animator>(); });
// }

void registerComponents() {
    auto& f = Factory::instance();

    f.registerComponent("transform", [] {
        return std::make_unique<Transform>();
    });

    f.registerComponent("sprite", [] {
        return std::make_unique<Sprite>();
    });

    f.registerComponent("animator", [] {
        return std::make_unique<Animator>();
    });

    f.registerComponent("rigidbody", [] {
        return std::make_unique<Rigidbody>();
    });
}

