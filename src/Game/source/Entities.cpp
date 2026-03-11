#include "Entities.h"

#include <cstdint>

#include "BreakoutECS.h"
#include "Components.h"
#include "SFML/System/Vector2.hpp"

namespace Breakout {

Breakout::ColliderComponent circleCollider({.circle = {8.0f}, .type = SHAPE_CIRCLE, .tag = PHYSICS_BALL});
Breakout::CollisionEvents circleCollisionEvents({{}, 0});

void RegisterBall(BreakoutECS& system, const Transform& transform, const RigidBody& rigidBody) {
        const BreakoutEntity entity = system.CreateEntity();
        system.AddComponent(entity, transform);
        system.AddComponent(entity, rigidBody);
        system.AddComponent(entity, Ball{});
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

Breakout::ColliderComponent WallCollider({.rect = {0.0f, 0.0f}, .type = SHAPE_RECTANGLE, .tag = PHYSICS_WALL});
Breakout::CollisionEvents WallsCollisionEvents({{}, 0});

void RegisterWalls(BreakoutECS& system, const sf::Vector2f& viewSize) {
        // Register the entities.
        BreakoutEntity entities[4];
        for (uint8_t idx = 0; idx < 4; idx++) {
                entities[idx] = system.CreateEntity();
        }

        float halfWidth = viewSize.x / 2.0f;
        float halfHeight = viewSize.y / 2.0f;

        sf::Vector2f location[4] = {
            {viewSize.x, halfHeight},  // Right
            {halfWidth, 0},            // Top
            {0, halfHeight},           // Left
            {halfWidth, viewSize.y}    // Bottom
        };

        sf::Vector2f velocity[4] = {
            {0, 0},  // Right
            {0, 0},  // Top
            {0, 0},  // Left
            {0, 0}   // Bottom
        };

        sf::Vector2f rectSize[4] = {
            {0, viewSize.y},  // Right
            {viewSize.x, 0},  // Top
            {0, viewSize.y},  // Left
            {viewSize.x, 0}   // Bottom
        };

        PhysicsTag tags[4] = {PHYSICS_WALL,       // Right
                              PHYSICS_WALL,       // Top
                              PHYSICS_WALL,       // Left
                              PHYSICS_KILLWALL};  // Bottom

        for (uint8_t idx = 0; idx < 4; idx++) {
                system.AddComponent(entities[idx], Transform(location[idx]));
                system.AddComponent(entities[idx], RigidBody(velocity[idx]));
                system.AddComponent(entities[idx], WallsCollisionEvents);

                WallCollider.rect.width = rectSize[idx].x;
                WallCollider.rect.height = rectSize[idx].y;
                WallCollider.tag = tags[idx];
                system.AddComponent(entities[idx], WallCollider);
        }
}

void RegisterLifes(BreakoutECS& system) {
        BreakoutEntity entities[3];
        for (uint32_t idx = 0; idx < 3; idx++) {
                entities[idx] = system.CreateEntity();
        }

        sf::Vector2f location[3] = {{20, 20}, {40, 20}, {60, 20}};

        for (uint32_t idx = 0; idx < 3; idx++) {
                system.AddComponent(entities[idx], Transform{location[idx]});
                system.AddComponent(entities[idx], Sprite({"assets/Textures/AvailableBall.png"}));
                system.AddComponent(entities[idx], BallSlots{false});
        }
}
}  // namespace Breakout
