#include "Entities.h"

#include "Components.h"

namespace Breakout {

Breakout::ColliderComponent circleCollider({.circle = {8.0f}, .type = SHAPE_CIRCLE, .tag = PHYSICS_BALL});
Breakout::CollisionEvents circleCollisionEvents({{}, 0});

void RegisterBall(BreakoutECS& system, Transform& transform, RigidBody& rigidBody) {
        const BreakoutEntity entity = system.CreateEntity();
        system.AddComponent(entity, transform);
        system.AddComponent(entity, rigidBody);
        system.AddComponent(entity, circleCollider);
        system.AddComponent(entity, circleCollisionEvents);
        system.AddComponent(entity, Sprite({Engine::ManagedSprite("assets/Textures/BreakoutBallv2.png")}));
}
}  // namespace Breakout
