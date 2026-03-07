#pragma once
#include <Engine/ECS/ECS.h>

#include "Breakout/BreakoutComponents.h"
#include "BreakoutECS.h"
#include "Components.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/System.h"

namespace Breakout {

class CollisionResolutionSystem : public Engine::System {
       public:
        void OnUpdate(BreakoutECS& system) {
                for (const auto entity : m_Entities) {
                        CollisionEvents& events = system.GetComponent<CollisionEvents>(entity);
                        ColliderComponent& shape = system.GetComponent<ColliderComponent>(entity);

                        for (uint8_t idx = 0; idx < events.eventCount; idx++) {
                                Engine::Entity other = events.hits[idx].other;
                                ColliderComponent& otherShape = system.GetComponent<ColliderComponent>(other);

                                m_CollisionMatrix[shape.tag][otherShape.tag](entity, events.hits[idx], system);
                        }

                        // Reset the count.
                        events.eventCount = 0;
                }
        }

        static void RegisterSelf(BreakoutECS& system) {
                BreakoutSignature signature;
                signature.set(system.GetComponentType<CollisionEvents>(), true);
                signature.set(system.GetComponentType<ColliderComponent>(), true);
                signature.set(system.GetComponentType<Transform>(), true);
                signature.set(system.GetComponentType<RigidBody>(), true);

                system.RegisterSystem<CollisionResolutionSystem>();
                system.SetSystemSignature<CollisionResolutionSystem>(signature);
        }

       private:
        using ResponseFunction = void (*)(const Engine::Entity entity, const CollisionResult& hit, BreakoutECS& system);

        static void BasicBounce(const Engine::Entity entity, const CollisionResult& hit, BreakoutECS& system);
        static void PaddleBounce(const Engine::Entity entity, const CollisionResult& hit, BreakoutECS& system);
        static void IgnoreBounce([[maybe_unused]] const Engine::Entity entity,
                                 [[maybe_unused]] const CollisionResult& hit, [[maybe_unused]] BreakoutECS& system);

        // The first is this this, second is other.
        static constexpr ResponseFunction m_CollisionMatrix[PhysicsTag::PHYSICS_COUNT][PhysicsTag::PHYSICS_COUNT] = {
            /* This/other        Ball,           Wall,        Paddle */
            /* Ball,      */ {IgnoreBounce, BasicBounce, PaddleBounce},
            /* Wall       */ {IgnoreBounce, IgnoreBounce, IgnoreBounce},
            /* Paddle     */ {IgnoreBounce, IgnoreBounce, IgnoreBounce}};
};
}  // namespace Breakout
