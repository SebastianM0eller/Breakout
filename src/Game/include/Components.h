#pragma once
#include <SFML/System/Vector2.hpp>
#include <array>
#include <cstdint>

namespace Breakout {

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

struct Transform {
        sf::Vector2f location;
};

struct ColliderComponent {
        union {
                CircleData circle;
                RectangleData rect;
        };
        ShapeType type;
};

struct WorldCollider {
        ColliderComponent& shape;
        Transform& transform;
};

struct CollisionResult {
        sf::Vector2f normal;
        float penetrationDepth;
        bool isColliding;
};

struct CollisionEvents {
        std::array<CollisionResult, 4> hits;
        uint8_t eventCount;
};

}  // namespace Breakout
