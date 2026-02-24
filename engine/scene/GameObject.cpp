// #include "scene/GameObject.h"
// #include "component/Factory.h"
//
// template<typename T, typename... Args>
// T& GameObject::addComponent(Args&&... args) {
//     static_assert(!std::is_same_v<T, Transform>);
//     auto comp = std::make_unique<T>(std::forward<Args>(args)...);
//     comp->owner = this;
//
//     T* ptr = comp.get();
//     components[typeid(T)] = std::move(comp);
//     ptr->OnCreate();
//     return *ptr;
// }
//
// template<typename T>
// T* GameObject::getComponent() {
//     if constexpr (std::is_same_v<T, Transform>)
//         return &transform;
//
//     auto it = components.find(typeid(T));
//     return it == components.end() ? nullptr : static_cast<T*>(it->second.get());
// }
//
// Component* GameObject::addComponent(const std::string& name) {
//     auto comp = Factory::instance().create(name);
//     if (!comp) return nullptr;
//
//     comp->owner = this;
//     auto type = std::type_index(typeid(*comp));
//     components[type] = std::move(comp);
//     components[type]->OnCreate();
//     return components[type].get();
// }
//
// void GameObject::Update(float dt) {
//     for (auto& [_, c] : components)
//         c->Update(dt);
// }
