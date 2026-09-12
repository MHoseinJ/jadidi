#pragma once
#include "utils/math/vector.h"
#include "lua/GameObjectHandle.h"
#include <vector>
#include <unordered_map>
#include "box2d/box2d.h"
#include <box2d/id.h>
#include <box2d/types.h>

enum class BodyType {
    Static,
    Dynamic,
    Kinematic
};

struct DebugShape {
    bool isTrigger;
    std::vector<Vector2> worldVertices;
};

struct Object {
    b2BodyId body;
    b2ShapeId shape;
    GameObjectHandle owner;
};

struct RaycastHit {
    bool hit = false;
    Vector2 point = {0.0f, 0.0f};
    Vector2 normal = {0.0f, 0.0f};
    float fraction = 0.0f;
    GameObjectHandle gameObject;
};

enum class PhysicsEventType {
    CollisionEnter,
    CollisionExit,
    TriggerEnter,
    TriggerExit
};

struct PhysicsEvent {
    PhysicsEventType type;
    GameObjectHandle self;
    GameObjectHandle other;
};

class Physics {
public:
    Physics(Vector2 gravity);
    ~Physics();

    void setGravity(Vector2 gravity);
    void setBodyType(Object* object, BodyType type);
    void setShapeDensity(Object* object, float density);
    void setShapeFriction(Object* object, float friction);

    Object createBody(BodyType type, Vector2 position, Vector2 scale = {1, 1},
                      float density = 1, float friction = 1, 
                      bool collision = true, bool isTrigger = false,
                      GameObjectHandle owner = GameObjectHandle{});

    Vector2 getPosition(Object* object);
    Vector2 getVelocity(Object* object);
    void setVelocity(Object* object, Vector2 velocity);
    void applyImpulse(Object* object, Vector2 impulse);
    void setPosition(Object* object, Vector2 position);

    RaycastHit raycast(Vector2 start, Vector2 end);

    void wakeTouchingBodies(Object* object);

    void collectEvents();
    const std::vector<PhysicsEvent>& getEvents() const; 

    std::vector<DebugShape> getDebugShapes() const;

    void deleteBody(Object object);
    void updatePhysics(float deltaTime);

private:
    b2WorldId world;
    std::vector<Object> objects;
    int worldStep = 4;

    std::vector<PhysicsEvent> pendingEvents;
    std::unordered_map<int, GameObjectHandle> shapeToOwner; 
};