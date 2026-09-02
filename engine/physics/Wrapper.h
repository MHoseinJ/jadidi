#pragma once

#include "utils/math/vector.h"
#include "vector"

#include "box2d/box2d.h"
#include <box2d/id.h>
#include <box2d/types.h>


enum BodyType {
    Static,
    Dynamic
};


struct Object {
    b2BodyId body;
    b2ShapeId shape;
};

class Physics {

    Physics();
    ~Physics();

    public:
        static Physics& get();
        void createBody(BodyType type, Vector2 position);
        void updatePhysics(float deltaTime);

    private:
        b2WorldId world;
        std::vector<Object> objects;
        int worldStep = 4;
    
};