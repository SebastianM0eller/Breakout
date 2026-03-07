#pragma once

#include <Engine/ECS/System.h>

#include "BreakoutECS.h"
#include "Components.h"

namespace Breakout {
class PhysicsSystem : public Engine::System {
       public:
        void OnUpdate(float deltaTime, BreakoutECS& system) {
                for (auto const entity : m_Entities) {
                        RigidBody& rigidBody = system.GetComponent<RigidBody>(entity);
                        Transform& transform = system.GetComponent<Transform>(entity);

                        transform.location.x += rigidBody.velocity.x * deltaTime;
                        transform.location.y += rigidBody.velocity.y * deltaTime;
                }
        }

        static void RegisterSelf(BreakoutECS& system) {
                BreakoutSignature signature;
                signature.set(system.GetComponentType<RigidBody>(), true);
                signature.set(system.GetComponentType<Transform>(), true);

                system.RegisterSystem<PhysicsSystem>();
                system.template SetSystemSignature<PhysicsSystem>(signature);
        }
};
}  // namespace Breakout
