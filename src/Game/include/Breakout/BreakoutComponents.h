#pragma once

#include "Engine/Resources/Texture.h"
#include "SFML/System/Vector2.hpp"

enum class ShapeType {
        BoxCollider,
        CircleCollider,
        PaddleCollider,
};

///
/// Hold the CollisionShape data.
/// We keep the CollisionShape in one struct for ease of use.
///
struct CollisionShape {
        ShapeType type;

        union {
                struct {
                        float radius;
                } circle;
                struct {
                        float width, height;
                } box;
        } shapeData;
};

///
/// Used to hold the sprite, we want to draw on screen.
/// Automatically manages the Texture resource.
///
struct Sprite {
        Engine::ManagedSprite sprite;
};

///
/// Hold a Vector2 descriping the location.
///
struct Transform {
        sf::Vector2f location;
};

///
/// Holds the velocity of an object.
///
struct RigidBody {
        sf::Vector2f velocity;
};
