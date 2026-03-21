#pragma once

#include <Engine/ECS/System.h>

#include <vector>

#include "BreakoutECS.h"
#include "Components.h"
#include "Engine/Math/Easing.h"

namespace Breakout {
class LerpingSystem : public Engine::System {
   public:
    void OnUpdate(BreakoutECS& system, float deltaTime) {
        // Keep track of what components finished their lerp.
        std::vector<BreakoutEntity> FinishedLerp;

        for (const auto entity : m_Entities) {
            Transform& transform = system.GetComponent<Transform>(entity);
            LerpComponent& lerp = system.GetComponent<LerpComponent>(entity);

            lerp.elapsedTime += deltaTime;

            // Check if it is finished now.
            if (lerp.elapsedTime > lerp.duration) {
                FinishedLerp.push_back(entity);

                // We just snap it to the target, and avoid the calculations.
                transform.location = lerp.target;
                continue;
            }

            // Duration should never be 0 here, due to the earlier if.
            float blending = lerp.elapsedTime / lerp.duration;

            auto LerpResult = [](float start, float target, float blend) {
                return Engine::Math::Lerp(
                    start, target, Engine::Math::EaseOutQuadratic(blend));
            };

            transform.location = {
                LerpResult(lerp.start.x, lerp.target.x, blending),
                LerpResult(lerp.start.y, lerp.target.y, blending)};
        }

        for (auto entity : FinishedLerp) {
            system.RemoveComponent<LerpComponent>(entity);
        }
    }

    static void RegisterSelf(BreakoutECS& system) {
        BreakoutSignature signature;
        signature.set(system.GetComponentType<Transform>(), true);
        signature.set(system.GetComponentType<LerpComponent>(), true);

        system.RegisterSystem<LerpingSystem>();
        system.SetSystemSignature<LerpingSystem>(signature);
    }
};
}  // namespace Breakout
