#pragma once

#include "SFML/System/Vector2.hpp"

struct Location {
        sf::Vector2f location;
};

struct RigidBody {
        sf::Vector2f velocity;
        sf::Vector2f acceleration;
};

struct LogLocation {};
