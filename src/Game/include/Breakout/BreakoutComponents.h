#pragma once

#include <vector>

#include "Engine/Resources/Texture.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"

enum class ShapeType {
        BoxCollider,
        CircleCollider,
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

struct HorizontalMovement {
        std::vector<sf::Keyboard::Key> moveRightKeys;
        std::vector<sf::Keyboard::Key> moveLeftKeys;
        float movementSpeed;
};
