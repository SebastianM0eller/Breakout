#pragma once
#include <Engine/ECS/Entity.h>
#include <Engine/Resources/ManagedFont.h>
#include <Engine/Resources/ManagedSprite.h>

#include <SFML/System/Vector2.hpp>
#include <array>
#include <cstdint>

#include "Engine/Resources/Text.h"

namespace Breakout {

struct Destroyed {};

template <typename T>
struct DestroyedComponent {};

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
    PHYSICS_REGULAR,
    PHYSICS_KILL,
    PHYSICS_PADDLE,
    PHYSICS_BOX,
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

struct Ball {
    uint32_t scoreIncrease{0};
    float speedMultiplier{1.001};
};

struct BallSlots {
    bool empty;
};

struct AvailableBallSpawn {};

struct Score {
    uint32_t score;
};

struct Text {
    Engine::Text text;
    Engine::ManagedFont
        font;  // Used to hold the lifetime for the font used in the text.
};

struct Box {};

struct LerpComponent {
    sf::Vector2f start;
    sf::Vector2f target;
    float duration;
    float elapsedTime;

    // Add different Easing types in the future.
};

}  // namespace Breakout
