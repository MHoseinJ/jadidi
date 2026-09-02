#include "Wrapper.h"
#include "lua/GameObjectHandle.h"
#include "utils/math/vector.h"
#include <algorithm>
#include <box2d/box2d.h>
#include <box2d/collision.h>
#include <box2d/id.h>
#include <box2d/math_functions.h>
#include <box2d/types.h>
#include <iostream>


Physics::Physics(Vector2 gravity) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {gravity.x, gravity.y};
    world = b2CreateWorld(&worldDef);
}


void Physics::createBody(BodyType type, Vector2 position, Vector2 scale, float density, float friction) {
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
    b2Polygon box = b2MakeBox(scale.x, scale.y);

    // shape def
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = density;
    shapeDef.material.friction = friction;

    // shape id
    b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);

    Object object = {bodyId, shapeId};
    objects.emplace_back(object);
}


void Physics::deleteBody(Object object) {
    b2DestroyBody(object.body);
    b2DestroyShape(object.shape, true);
    
    objects.erase(
        std::remove_if(objects.begin(), objects.end(), [&object](const Object obj) {
            return obj.body.index1 == object.body.index1 &&
                   obj.body.generation == object.body.generation;
        }),
        objects.end()
    );
}


void Physics::updatePhysics(float deltaTime) {
    b2World_Step(world, deltaTime, worldStep);
    for (auto& object : objects) {
        b2Vec2 position = b2Body_GetPosition(object.body);
        // TODO: add control of changing position
    }
}


Physics::~Physics() {
    b2DestroyWorld(world);
}