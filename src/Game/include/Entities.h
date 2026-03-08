#pragma once

#include "BreakoutECS.h"
#include "Components.h"
#include "SFML/System/Vector2.hpp"

namespace Breakout {
void RegisterBall(BreakoutECS& system, const Transform& transform, const RigidBody& rigidBody);
void RegisterPaddle(BreakoutECS& system, const Transform& transsform);
void RegisterWalls(BreakoutECS& system, const sf::Vector2f& viewSize);
}  // namespace Breakout
