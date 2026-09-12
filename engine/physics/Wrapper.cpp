#include "Wrapper.h"
#include "lua/GameObjectHandle.h"
#include "utils/math/vector.h"
#include <algorithm>
#include <box2d/box2d.h>
#include <box2d/collision.h>
#include <box2d/id.h>
#include <box2d/math_functions.h>
#include <box2d/types.h>
#include "iostream"

Physics::Physics(Vector2 gravity) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {gravity.x, gravity.y};
    world = b2CreateWorld(&worldDef);
}

void Physics::setGravity(Vector2 gravity) {
    b2World_SetGravity(world, {gravity.x, gravity.y});
}

Object Physics::createBody(BodyType type, Vector2 position, Vector2 scale,
                           float density, float friction, 
                           bool collision, bool isTrigger,
                           GameObjectHandle owner) {
                               
    b2BodyDef bodyDef = b2DefaultBodyDef();
    switch (type) {
        case BodyType::Static:
            bodyDef.type = b2_staticBody;
            break;
        case BodyType::Dynamic:
            bodyDef.type = b2_dynamicBody;
            break;
        case BodyType::Kinematic:
            bodyDef.type = b2_kinematicBody;
            break;
        default:
            bodyDef.type = b2_staticBody;
    }
    bodyDef.position = (b2Vec2){position.x, position.y};
    b2BodyId bodyId = b2CreateBody(world, &bodyDef);

    b2ShapeId shapeId = b2_nullShapeId;
    if (collision) {
        b2Polygon box = b2MakeBox(scale.x/2, scale.y/2);
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = density;
        shapeDef.material.friction = friction;
        shapeDef.isSensor = isTrigger;
        shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);
        
        if (b2Shape_IsValid(shapeId)) {
            shapeToOwner[shapeId.index1] = owner;
        }
    }

    Object object = {bodyId, shapeId, owner};
    objects.emplace_back(object);
    return objects.back();
}

void Physics::deleteBody(Object object) {
    
    if (b2Shape_IsValid(object.shape)) {
        shapeToOwner.erase(object.shape.index1);
    }
    
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
    switch (type) {
        case BodyType::Static:
            b2Body_SetType(object->body, b2_staticBody);
            break;
        case BodyType::Dynamic:
            b2Body_SetType(object->body, b2_dynamicBody);
            break;
        case BodyType::Kinematic:
            b2Body_SetType(object->body, b2_kinematicBody);
            break;
    }
}

void Physics::setShapeDensity(Object* object, float density) {
    if (!b2Shape_IsValid(object->shape)) return;
    b2Shape_SetDensity(object->shape, density, true);
}

void Physics::setShapeFriction(Object* object, float friction) {
    if (!b2Shape_IsValid(object->shape)) return;
    b2Shape_SetFriction(object->shape, friction);
}

void Physics::setPosition(Object* object, Vector2 position) {
    if (!b2Body_IsValid(object->body))
        return;

    b2Body_SetTransform(
        object->body,
        {position.x, position.y},
        b2Body_GetRotation(object->body)
    );

    wakeTouchingBodies(object);
}

void Physics::collectEvents() {
    pendingEvents.clear();
    
    b2ContactEvents contactEvents = b2World_GetContactEvents(world);
    
    for (int i = 0; i < contactEvents.beginCount; i++) {
        b2ContactBeginTouchEvent& e = contactEvents.beginEvents[i];
        auto it1 = shapeToOwner.find(e.shapeIdA.index1);
        auto it2 = shapeToOwner.find(e.shapeIdB.index1);
        if (it1 != shapeToOwner.end() && it2 != shapeToOwner.end()) {

            pendingEvents.push_back({PhysicsEventType::CollisionEnter, it1->second, it2->second});
            pendingEvents.push_back({PhysicsEventType::CollisionEnter, it2->second, it1->second});
        }
    }
    
    for (int i = 0; i < contactEvents.endCount; i++) {
        b2ContactEndTouchEvent& e = contactEvents.endEvents[i];
        auto it1 = shapeToOwner.find(e.shapeIdA.index1);
        auto it2 = shapeToOwner.find(e.shapeIdB.index1);
        if (it1 != shapeToOwner.end() && it2 != shapeToOwner.end()) {
            pendingEvents.push_back({PhysicsEventType::CollisionExit, it1->second, it2->second});
            pendingEvents.push_back({PhysicsEventType::CollisionExit, it2->second, it1->second});
        }
    }

    b2SensorEvents sensorEvents = b2World_GetSensorEvents(world);
    
    for (int i = 0; i < sensorEvents.beginCount; i++) {
        b2SensorBeginTouchEvent& e = sensorEvents.beginEvents[i];
        auto itSensor = shapeToOwner.find(e.sensorShapeId.index1);
        auto itVisitor = shapeToOwner.find(e.visitorShapeId.index1);
        if (itSensor != shapeToOwner.end() && itVisitor != shapeToOwner.end()) {
            pendingEvents.push_back({PhysicsEventType::TriggerEnter, itSensor->second, itVisitor->second});
            pendingEvents.push_back({PhysicsEventType::TriggerEnter, itVisitor->second, itSensor->second});
        }
    }
    
    for (int i = 0; i < sensorEvents.endCount; i++) {
        b2SensorEndTouchEvent& e = sensorEvents.endEvents[i];
        auto itSensor = shapeToOwner.find(e.sensorShapeId.index1);
        auto itVisitor = shapeToOwner.find(e.visitorShapeId.index1);
        if (itSensor != shapeToOwner.end() && itVisitor != shapeToOwner.end()) {
            pendingEvents.push_back({PhysicsEventType::TriggerExit, itSensor->second, itVisitor->second});
            pendingEvents.push_back({PhysicsEventType::TriggerExit, itVisitor->second, itSensor->second});
        }
    }
}

const std::vector<PhysicsEvent>& Physics::getEvents() const {
    return pendingEvents;
}

void Physics::wakeTouchingBodies(Object* object) {
    if (!object || !b2Body_IsValid(object->body))
        return;

    const int capacity = b2Body_GetContactCapacity(object->body);

    if (capacity == 0)
        return;

    std::vector<b2ContactData> contacts(capacity);

    const int count = b2Body_GetContactData(
        object->body,
        contacts.data(),
        capacity
    );

    for (int i = 0; i < count; ++i) {
        const b2BodyId bodyA =
            b2Shape_GetBody(contacts[i].shapeIdA);

        const b2BodyId bodyB =
            b2Shape_GetBody(contacts[i].shapeIdB);

        b2BodyId otherBody;

        if (B2_ID_EQUALS(bodyA, object->body))
            otherBody = bodyB;
        else
            otherBody = bodyA;

        if (b2Body_GetType(otherBody) == b2_dynamicBody)
            b2Body_SetAwake(otherBody, true);
    }
}
Physics::~Physics() {
    b2DestroyWorld(world);
}