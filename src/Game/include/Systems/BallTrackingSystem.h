#pragma once

#include <Engine/ECS/System.h>

#include "BreakoutECS.h"
#include "Components.h"
#include "Events.h"

namespace Breakout {
class BallTrackingSystem : public Engine::System {
   public:
    void OnUpdate(BreakoutECS& system) {
        if (m_CheckBallAmount) {
            if (m_Entities.empty())
                system.SendEvent(LifeUsedEvent{});

            m_CheckBallAmount = false;
        }
    }
    void OnBallDestruction() {
        m_CheckBallAmount = true;
    }

    static void RegisterSelf(BreakoutECS& system) {
        BreakoutSignature signature;
        signature.set(system.GetComponentType<Ball>(), true);

        auto self = system.RegisterSystem<BallTrackingSystem>();
        system.SetSystemSignature<BallTrackingSystem>(signature);

        std::weak_ptr<BallTrackingSystem> weakSelf = self;

        system.AddListner<BallDestroyedEvent>(
            [weakSelf](const BallDestroyedEvent&) {
                if (auto lockedSelf = weakSelf.lock()) {
                    lockedSelf->OnBallDestruction();
                }
            });
    }

   private:
    bool m_CheckBallAmount{false};
};
}  // namespace Breakout
