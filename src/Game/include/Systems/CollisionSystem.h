#pragma once
#include <Engine/ECS/ECS.h>

#include <cstdint>

#include "Components.h"
#include "Engine/ECS/System.h"

namespace Breakout {
class CollisionSystem : public Engine::System {
       public:
        template <uint8_t ComponentCount, uint32_t EntityCount>
        void OnUpdate(Engine::ECS<ComponentCount, EntityCount>& system) {
                for (auto iteratorA = m_Entities.begin(); iteratorA != m_Entities.end(); iteratorA++) {
                        auto iteratorB = iteratorA;
                        iteratorB++;  // Increment to avoid checking collision with yourself.

                        uint32_t a = *iteratorA;
                        Transform& aTransform = system.template GetComponent<Transform>(a);
                        ColliderComponent& aShape = system.template GetComponent<ColliderComponent>(a);

                        for (; iteratorB != m_Entities.end(); iteratorB++) {
                                uint32_t b = *iteratorB;

                                Transform& bTransform = system.template GetComponent<Transform>(b);
                                ColliderComponent& bShape = system.template GetComponent<ColliderComponent>(b);

                                WorldCollider aWorld = {aShape, aTransform};
                                WorldCollider bWorld = {bShape, bTransform};

                                CollisionResult result = m_CollisionMatrix[aShape.type][bShape.type](aWorld, bWorld);

                                if (result.isColliding) {
                                        CollisionResult otherResult = result;
                                        otherResult.normal = -otherResult.normal;

                                        CollisionEvents& aEvents = system.template GetComponent<CollisionEvents>(a);
                                        CollisionEvents& bEvents = system.template GetComponent<CollisionEvents>(b);

                                        if (aEvents.eventCount < 4) {
                                                aEvents.hits[aEvents.eventCount] = result;
                                                ++aEvents.eventCount;
                                        }

                                        if (bEvents.eventCount < 4) {
                                                bEvents.hits[bEvents.eventCount] = otherResult;
                                                ++bEvents.eventCount;
                                        }
                                }
                        }
                }
        }

        template <uint8_t ComponentCount, uint32_t EntityCount>
        void RegisterSelf(Engine::ECS<ComponentCount, EntityCount>& system) {
                using Signature = std::bitset<ComponentCount>;

                Signature signature;
                signature.set(system.template GetComponentType<ColliderComponent>(), true);
                signature.set(system.template GetComponentType<CollisionEvents>(), true);
                signature.set(system.template GetComponentType<Transform>(), true);

                system.template RegisterSystem<CollisionSystem>();
                system.template SetSystemSignature<CollisionSystem>(signature);
        }

       private:
        using Result = Breakout::CollisionResult;
        using CollisionFunction = Result (*)(const WorldCollider& a, const WorldCollider& b);

        static Result CollideCircleCircle(const Breakout::WorldCollider& a, const Breakout::WorldCollider& b);
        static Result CollideCircleRect(const Breakout::WorldCollider& a, const Breakout::WorldCollider& b);
        static Result CollideRectCircle(const Breakout::WorldCollider& a, const Breakout::WorldCollider& b);
        static Result CollideRectRect(const Breakout::WorldCollider& a, const Breakout::WorldCollider& b);

        static constexpr CollisionFunction m_CollisionMatrix[ShapeType::SHAPE_TYPE_COUNT][ShapeType::SHAPE_TYPE_COUNT] =
            {{CollideCircleCircle, CollideCircleRect}, {CollideRectCircle, CollideRectRect}};
};
}  // namespace Breakout
