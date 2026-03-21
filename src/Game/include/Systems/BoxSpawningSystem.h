#pragma once

#include <Engine/ECS/System.h>

#include <iostream>

#include "BreakoutECS.h"
#include "Components.h"
#include "Engine/Renderer.h"
#include "Entities.h"
#include "Events.h"

namespace Breakout {
class BoxSpawningSystem : public Engine::System {
   public:
    void OnUpdate(BreakoutECS& system) {
        if (m_CheckBallAmount) {
            if (m_Entities.empty()) {
                // Do Box Spawning.
                // The position should have some lerp, to move them into the
                // scene.

                // For now i will just spawn a single box.
                RegisterBox(system,
                            Engine::Renderer::Get().GetViewSize() / 2.0f);

                // Debug:
                std::cout << "New Box Spawned!";
            }
            m_CheckBallAmount = false;
        }
    }

    static void RegisterSelf(BreakoutECS& system) {
        BreakoutSignature signature;
        signature.set(system.GetComponentType<Box>(), true);

        auto self = system.RegisterSystem<BoxSpawningSystem>();
        system.SetSystemSignature<BoxSpawningSystem>(signature);

        std::weak_ptr<BoxSpawningSystem> weakSelf = self;

        system.AddListner<BoxDestroyedEvent>(
            [weakSelf](const BoxDestroyedEvent&) {
                if (auto lockedSelf = weakSelf.lock()) {
                    lockedSelf->m_CheckBallAmount = true;
                }
            });
    }

   private:
    bool m_CheckBallAmount{true};
};
}  // namespace Breakout
