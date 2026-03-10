#pragma once

#include "BreakoutECS.h"
#include "Components.h"
namespace Breakout {
class LocationSyncSystem : public Engine::System {
       public:
        void OnUpdate(BreakoutECS& system) {
                for (auto const entity : m_Entities) {
                        Transform& transform = system.GetComponent<Transform>(entity);
                        Sprite& sprite = system.GetComponent<Sprite>(entity);

                        sprite.sprite.SetPosition(transform.location);
                }
        }

        static void RegisterSelf(BreakoutECS& system) {
                BreakoutSignature signature;
                signature.set(system.GetComponentType<Breakout::Transform>(), true);
                signature.set(system.GetComponentType<Breakout::Sprite>(), true);

                system.RegisterSystem<LocationSyncSystem>();
                system.SetSystemSignature<LocationSyncSystem>(signature);
        }
};
}  // namespace Breakout
