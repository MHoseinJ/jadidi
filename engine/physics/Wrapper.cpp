#include "Wrapper.h"
#include "lua/GameObjectHandle.h"
#include "utils/math/vector.h"
#include <box2d/box2d.h>
#include <box2d/collision.h>
#include <box2d/id.h>
#include <box2d/math_functions.h>
#include <box2d/types.h>
#include <iostream>


Physics::Physics() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    world = b2CreateWorld(&worldDef);
}


Physics& Physics::get() {
    static Physics phys;
    return phys;
}

void Physics::createBody(BodyType type, Vector2 position) {
    // body def
    b2BodyDef bodyDef = b2DefaultBodyDef();
    switch (type) {
        case Static:
            bodyDef.type = b2_staticBody;
            break;

        case Dynamic:
            bodyDef.type = b2_dynamicBody;
            break;

        default:
            bodyDef.type = b2_staticBody;
    }
    bodyDef.position = (b2Vec2){position.x, position.y};
    b2BodyId bodyId = b2CreateBody(world, &bodyDef);

    // box
    b2Polygon box = b2MakeBox(1, 1);

    // shape def
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1;
    shapeDef.material.friction = 1;

    // shape id
    b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);

    Object object = {bodyId, shapeId};
    objects.emplace_back(object);
}


void Physics::updatePhysics(float deltaTime) {
    b2World_Step(world, deltaTime, worldStep);
    for (auto& object : objects) {
        b2Vec2 position = b2Body_GetPosition(object.body);
        std::cout <<"position: " << position.x  << ", " << position.y << std::endl;
    }
}


Physics::~Physics() {
    b2DestroyWorld(world);
}