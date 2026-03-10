#pragma once

#include <Engine/ECS/System.h>

#include "BreakoutECS.h"
#include "Components.h"
#include "Events.h"
namespace Breakout {
class BallTrackingSystem : public Engine::System {
       public:
        void OnUpdate(BreakoutECS& system) {
                if (m_Entities.empty()) {
                        system.SendEvent(LifeUsedEvent{});
                }
        }

        static void RegisterSelf(BreakoutECS& system) {
                BreakoutSignature signature;
                signature.set(system.GetComponentType<Ball>(), true);

                system.RegisterSystem<BallTrackingSystem>();
                system.SetSystemSignature<BallTrackingSystem>(signature);
        }
};
}  // namespace Breakout
