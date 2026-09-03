#pragma once

#include "utils/math/vector.h"
#include <vector>

#include "box2d/box2d.h"
#include <box2d/id.h>
#include <box2d/types.h>


enum class BodyType {
    Static,
    Dynamic
};


struct Object {
    b2BodyId body;
    b2ShapeId shape;
};

class Physics {

    public:

        Physics(Vector2 gravity);
        ~Physics();
        
        Object createBody(BodyType type, Vector2 position, Vector2 scale = {1, 1}, float density = 1, float friction = 1, bool collision = true);
        Vector2 getPosition(Object* object);
        void deleteBody(Object object);
        void updatePhysics(float deltaTime);

    private:
        b2WorldId world;
        std::vector<Object> objects;
        int worldStep = 4;
    
};