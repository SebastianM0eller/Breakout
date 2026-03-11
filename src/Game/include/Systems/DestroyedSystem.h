#pragma once

#include <Engine/ECS/System.h>

#include <vector>

#include "BreakoutECS.h"
#include "Components.h"
#include "Engine/ECS/Entity.h"

namespace Breakout {
class DestroyedSystem : public Engine::System {
       public:
        void OnUpdate(BreakoutECS& system) {
                std::vector<Engine::Entity> entitiesToDestroy(m_Entities.begin(), m_Entities.end());
                for (const Engine::Entity entity : entitiesToDestroy) {
                        system.DestroyEntity(entity);
                }
        }

        static void RegisterSelf(BreakoutECS& system) {
                BreakoutSignature signature;
                signature.set(system.GetComponentType<Destroyed>(), true);

                system.RegisterSystem<DestroyedSystem>();
                system.SetSystemSignature<DestroyedSystem>(signature);
        }
};
}  // namespace Breakout
