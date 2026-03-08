#pragma once

#include "BreakoutECS.h"
#include "Components.h"

namespace Breakout {
void RegisterBall(BreakoutECS& system, Transform& transform, RigidBody& rigidBody);
void RegisterPaddle(BreakoutECS& system);
void RegisterWalls(BreakoutECS& system);
}  // namespace Breakout
