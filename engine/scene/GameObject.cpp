#include "GameObject.h"
#include "component/Transform.h"

bool GameObject::setParent(GameObject* newParent, bool)
{
    if (newParent == this)
        return false;
    if (newParent && newParent->isDescendantOf(this))
        return false;

    if (parent == newParent)
        return true;

    if (parent) {
        parent->removeChild(this);
    }
    
    parent = newParent;
    
    if (parent) {
        parent->children.push_back(this);
    }
    
    transform.markDirty();
    
    return true;
}

void GameObject::removeChild(GameObject* child)
{
    if (!child)
        return;

    auto it = std::find(children.begin(), children.end(), child);

    if (it != children.end()) {
        children.erase(it);

        if (child->parent == this) {
            child->parent = nullptr;
        }
    }
}

bool GameObject::isChildOf(const GameObject* object) const
{
    return parent == object;
}

bool GameObject::isDescendantOf(const GameObject* object) const
{
    if (!object)
        return false;

    const GameObject* current = parent;

    while (current) {
        if (current == object)
            return true;

        current = current->parent;
    }

    return false;
}