#pragma once
#include <SFML/System/Vector2.hpp>
#include <array>
#include <cstdint>

#include "Engine/ECS/Entity.h"
#include "Engine/Resources/Texture.h"

namespace Breakout {

struct Player {};

struct Transform {
        sf::Vector2f location;
};

struct RigidBody {
        sf::Vector2f velocity;
};

///
/// Automatically manages the Texture resource.
///
struct Sprite {
        Engine::ManagedSprite sprite;
};

///
/// The Physics tags. The PHYSICS_COUNT gives us the amount of types.
///
enum PhysicsTag : uint8_t {
        PHYSICS_BALL,
        PHYSICS_WALL,
        PHYSICS_PADDLE,
        PHYSICS_COUNT,
};

///
/// The shape tags. The SHAPE_TYPE_COUNT gives us the amount of types.
///
enum ShapeType : uint8_t {
        SHAPE_CIRCLE = 0,
        SHAPE_RECTANGLE,
        SHAPE_TYPE_COUNT,
};

struct CircleData {
        float radius;
};

struct RectangleData {
        float width;
        float height;
};

struct ColliderComponent {
        union {
                CircleData circle;
                RectangleData rect;
        };
        ShapeType type;
        PhysicsTag tag;
};

struct CollisionResult {
        sf::Vector2f normal;
        Engine::Entity other;
        float penetrationDepth;
        bool isColliding;
};

struct CollisionEvents {
        std::array<CollisionResult, 4> hits;
        uint8_t eventCount;
};

}  // namespace Breakout
