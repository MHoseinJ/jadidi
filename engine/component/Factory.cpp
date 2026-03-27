#include "component/Factory.h"

#include "Button.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Text.h"

void Factory::registerComponent(std::string_view name, Creator fn) {
    creators.emplace(name, std::move(fn));
}

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

    f.registerComponent("text", [] {
        return std::make_unique<Text>();
    });

    f.registerComponent("boxCollider", [] {
        return std::make_unique<BoxCollider>();
    });

    f.registerComponent("button", [] {
        return std::make_unique<Button>();
    });
}

