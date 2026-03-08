#include "Entities.h"

#include "BreakoutECS.h"
#include "Components.h"

namespace Breakout {

Breakout::ColliderComponent circleCollider({.circle = {8.0f}, .type = SHAPE_CIRCLE, .tag = PHYSICS_BALL});
Breakout::CollisionEvents circleCollisionEvents({{}, 0});

void RegisterBall(BreakoutECS& system, const Transform& transform, const RigidBody& rigidBody) {
        const BreakoutEntity entity = system.CreateEntity();
        system.AddComponent(entity, transform);
        system.AddComponent(entity, rigidBody);
        system.AddComponent(entity, circleCollider);
        system.AddComponent(entity, circleCollisionEvents);
        system.AddComponent(entity, Sprite({Engine::ManagedSprite("assets/Textures/BreakoutBallv2.png")}));
}

Breakout::ColliderComponent paddleCollider({.rect = {56.0f, 6.0f}, .type = SHAPE_RECTANGLE, .tag = PHYSICS_PADDLE});
Breakout::CollisionEvents paddleCollisionEvents({{}, 0});

void RegisterPaddle(BreakoutECS& system, const Transform& transform) {
        const BreakoutEntity entity = system.CreateEntity();
        system.AddComponent(entity, transform);
        system.AddComponent(entity, Player{});
        system.AddComponent(entity, RigidBody({0.0f, 0.0f}));
        system.AddComponent(entity, paddleCollider);
        system.AddComponent(entity, paddleCollisionEvents);
        system.AddComponent(entity, Sprite({Engine::ManagedSprite("assets/Textures/BreakoutPaddle.png")}));
}

void RegisterWalls(BreakoutECS& system, const sf::Vector2f& viewSize) {
        const BreakoutEntity entity = system.CreateEntity();

        float halfWidth = viewSize.x / 2.0f;
        float halfHeight = viewSize.y / 2.0f;
}
}  // namespace Breakout
