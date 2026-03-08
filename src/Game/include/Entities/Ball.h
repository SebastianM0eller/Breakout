#pragma once

#include "BreakoutECS.h"
#include "Components.h"

namespace Breakout {
void RegisterBall(BreakoutECS& system, Transform& transform, RigidBody& rigidBody) {
        const BreakoutEntity entity = system.CreateEntity();
        system.AddComponent(entity, transform);
}
}  // namespace Breakout
