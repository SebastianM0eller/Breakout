#pragma once

#include "BreakoutECS.h"
#include "Components.h"
#include "Engine/ECS/Entity.h"
#include "SFML/System/Vector2.hpp"

namespace Breakout {
void RegisterBall(BreakoutECS& system, const Transform& transform,
                  const RigidBody& rigidBody);
void RegisterPaddle(BreakoutECS& system, const Transform& transsform);
void RegisterWalls(BreakoutECS& system, const sf::Vector2f& viewSize);
void RegisterLifes(BreakoutECS& system);
void RegisterScore(BreakoutECS& system, sf::Vector2f viewSize);
Engine::Entity RegisterBox(BreakoutECS& system, sf::Vector2f location);
void RegisterBoxes(BreakoutECS& system);
}  // namespace Breakout
