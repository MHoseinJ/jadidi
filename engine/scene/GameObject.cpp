#include "GameObject.h"
#include "component/Transform.h"

bool GameObject::setParent(GameObject* newParent, bool keepWorldTransform)
{
    if (newParent == this)
        return false;

    if (newParent && newParent->isDescendantOf(this))
        return false;

    if (parent == newParent)
        return true;

    Matrix3 oldWorldMatrix;

    if (keepWorldTransform) {
        oldWorldMatrix = transform.getWorldMatrix();
    }

    if (parent) {
        parent->removeChild(this);
    }

    parent = newParent;

    if (parent) {
        parent->children.push_back(this);
    }

    if (keepWorldTransform) {
        Matrix3 newLocalMatrix = Matrix3::identity();

        if (parent) {
            newLocalMatrix =
                parent->transform.getWorldMatrix().inverse()
                * oldWorldMatrix;
        }
        else {
            newLocalMatrix = oldWorldMatrix;
        }

        Vector2 position{
            newLocalMatrix.m[0][2],
            newLocalMatrix.m[1][2]
        };

        transform.setLocalPosition(position);
    }
    else {
        transform.markDirty();
    }

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

void GameObject::detachFromHierarchy()
{
    if (parent) {
        parent->removeChild(this);
    }

    for (GameObject* child : children) {
        if (child && child->parent == this) {
            child->parent = nullptr;
            child->transform.markDirty();
        }
    }

    children.clear();
}