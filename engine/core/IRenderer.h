#pragma once

#include <vector>

class GameObject;
class Camera;

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual void init() = 0;
    virtual void beginFrame() = 0;
    virtual void drawScene(std::vector<GameObject>& objects, const Camera& camera) = 0;
    virtual void endFrame() = 0;
    
    virtual void markDirty() = 0; 
};