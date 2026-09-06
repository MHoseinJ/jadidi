#include "Wrapper.h"
#include "lua/GameObjectHandle.h"
#include "utils/math/vector.h"
#include <algorithm>
#include <box2d/box2d.h>
#include <box2d/collision.h>
#include <box2d/id.h>
#include <box2d/math_functions.h>
#include <box2d/types.h>

Physics::Physics(Vector2 gravity) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {gravity.x, gravity.y};
    world = b2CreateWorld(&worldDef);
}

void Physics::setGravity(Vector2 gravity) {
    b2World_SetGravity(world, {gravity.x, gravity.y});
}

Object Physics::createBody(BodyType type, Vector2 position, Vector2 scale,
                           float density, float friction, bool collision,
                           GameObjectHandle owner) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    switch (type) {
        case BodyType::Static:
            bodyDef.type = b2_staticBody;
            break;
        case BodyType::Dynamic:
            bodyDef.type = b2_dynamicBody;
            break;
        default:
            bodyDef.type = b2_staticBody;
    }
    bodyDef.position = (b2Vec2){position.x, position.y};
    b2BodyId bodyId = b2CreateBody(world, &bodyDef);

    b2ShapeId shapeId = b2_nullShapeId;
    if (collision) {
        b2Polygon box = b2MakeBox(scale.x, scale.y);
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = density;
        shapeDef.material.friction = friction;
        shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);
    }

    Object object = {bodyId, shapeId, owner};
    objects.emplace_back(object);
    return objects.back();
}

void Physics::deleteBody(Object object) {
    b2DestroyBody(object.body);
    objects.erase(
        std::remove_if(objects.begin(), objects.end(), [&object](const Object& obj) {
            return obj.body.index1 == object.body.index1 &&
                   obj.body.generation == object.body.generation;
        }),
        objects.end()
    );
}

void Physics::updatePhysics(float deltaTime) {
    b2World_Step(world, deltaTime, worldStep);
}

Vector2 Physics::getPosition(Object* object) {
    b2Vec2 position = b2Body_GetPosition(object->body);
    return {position.x, position.y};
}

Vector2 Physics::getVelocity(Object* object) {
    b2Vec2 vel = b2Body_GetLinearVelocity(object->body);
    return {vel.x, vel.y};
}

void Physics::setVelocity(Object* object, Vector2 velocity) {
    b2Body_SetLinearVelocity(object->body, {velocity.x, velocity.y});
}

void Physics::applyImpulse(Object* object, Vector2 impulse) {
    if (!b2Body_IsValid(object->body)) return;
    b2Body_ApplyLinearImpulseToCenter(object->body, {impulse.x, impulse.y}, true);
}

RaycastHit Physics::raycast(Vector2 start, Vector2 end) {
    RaycastHit result;

    b2Vec2 origin = {start.x, start.y};
    b2Vec2 translation = {end.x - start.x, end.y - start.y};
    b2QueryFilter filter = b2DefaultQueryFilter();

    b2RayResult hit = b2World_CastRayClosest(world, origin, translation, filter);

    if (hit.hit && b2Shape_IsValid(hit.shapeId)) {
        result.hit = true;
        result.point = {hit.point.x, hit.point.y};
        result.normal = {hit.normal.x, hit.normal.y};
        result.fraction = hit.fraction;

        for (const auto& obj : objects) {
            if (obj.shape.index1 == hit.shapeId.index1 &&
                obj.shape.generation == hit.shapeId.generation) {
                result.gameObject = obj.owner;
                break;
            }
        }
    }

    return result;
}

void Physics::setBodyType(Object* object, BodyType type) {
    if (!b2Body_IsValid(object->body)) return;
    b2Body_SetType(object->body, type == BodyType::Dynamic ? b2_dynamicBody : b2_staticBody);
}

void Physics::setShapeDensity(Object* object, float density) {
    if (!b2Shape_IsValid(object->shape)) return;
    b2Shape_SetDensity(object->shape, density, true);
}

void Physics::setShapeFriction(Object* object, float friction) {
    if (!b2Shape_IsValid(object->shape)) return;
    b2Shape_SetFriction(object->shape, friction);
}

Physics::~Physics() {
    b2DestroyWorld(world);
}