#pragma once

#include <Engine/ECS/System.h>

#include "BreakoutECS.h"
#include "Components.h"
#include "Events.h"

namespace Breakout {
class BallLifeSystem : public Engine::System {
       public:
        void LifeUsed(BreakoutECS& system);
        void LifeGained(BreakoutECS& system);
        void SendNewLifeAmountEvent(BreakoutECS& system);

        static void RegisterSelf(BreakoutECS& system) {
                BreakoutSignature signature;
                signature.set(system.GetComponentType<BallSlots>(), true);
                signature.set(system.GetComponentType<Sprite>(), true);
                signature.set(system.GetComponentType<Transform>(), true);

                // We capture the shared_ptr, to use it for the callbacks.
                auto self = system.RegisterSystem<BallLifeSystem>();
                system.SetSystemSignature<BallLifeSystem>(signature);

                // Register the callbacks using a weak_ptr for lifetime.
                std::weak_ptr<BallLifeSystem> weakSelf = self;

                system.AddListner<LifeGainedEvent>([weakSelf, &system](const LifeGainedEvent&) {
                        if (auto lockedSelf = weakSelf.lock()) {
                                lockedSelf->LifeGained(system);
                        }
                });

                system.AddListner<LifeUsedEvent>([weakSelf, &system](const LifeUsedEvent&) {
                        if (auto lockedSelf = weakSelf.lock()) {
                                lockedSelf->LifeUsed(system);
                        }
                });
        }
};
}  // namespace Breakout
