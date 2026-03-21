#pragma once
#include <Engine/ECS/ECS.h>

#include <cstdint>

#include "BreakoutECS.h"
#include "Components.h"

namespace Breakout {

struct WorldCollider {
    ColliderComponent& shape;
    Transform& transform;
};

class CollisionDetectionSystem : public Engine::System {
   public:
    void OnUpdate(BreakoutECS& system) {
        for (auto iteratorA = m_Entities.begin(); iteratorA != m_Entities.end();
             iteratorA++) {
            auto iteratorB = iteratorA;
            iteratorB++;  // Increment to avoid checking collision with
                          // yourself.

            uint32_t a = *iteratorA;
            Transform& aTransform = system.GetComponent<Transform>(a);
            ColliderComponent& aShape =
                system.GetComponent<ColliderComponent>(a);

            for (; iteratorB != m_Entities.end(); iteratorB++) {
                uint32_t b = *iteratorB;

                Transform& bTransform = system.GetComponent<Transform>(b);
                ColliderComponent& bShape =
                    system.GetComponent<ColliderComponent>(b);

                WorldCollider aWorld = {aShape, aTransform};
                WorldCollider bWorld = {bShape, bTransform};

                CollisionResult result =
                    m_CollisionMatrix[aShape.type][bShape.type](aWorld, bWorld);

                if (result.isColliding) {
                    CollisionResult otherResult = result;
                    otherResult.normal = -otherResult.normal;

                    result.other = b;
                    otherResult.other = a;

                    CollisionEvents& aEvents =
                        system.GetComponent<CollisionEvents>(a);
                    CollisionEvents& bEvents =
                        system.GetComponent<CollisionEvents>(b);

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

    static void RegisterSelf(BreakoutECS& system) {
        BreakoutSignature signature;
        signature.set(system.GetComponentType<ColliderComponent>(), true);
        signature.set(system.GetComponentType<CollisionEvents>(), true);
        signature.set(system.GetComponentType<Transform>(), true);

        system.RegisterSystem<CollisionDetectionSystem>();
        system.SetSystemSignature<CollisionDetectionSystem>(signature);
    }

   private:
    using Result = Breakout::CollisionResult;
    using CollisionFunction = Result (*)(const WorldCollider& a,
                                         const WorldCollider& b);

    static Result CollideCircleCircle(const Breakout::WorldCollider& a,
                                      const Breakout::WorldCollider& b);
    static Result CollideCircleRect(const Breakout::WorldCollider& a,
                                    const Breakout::WorldCollider& b);
    static Result CollideRectCircle(const Breakout::WorldCollider& a,
                                    const Breakout::WorldCollider& b);
    static Result CollideRectRect(const Breakout::WorldCollider& a,
                                  const Breakout::WorldCollider& b);

    static constexpr CollisionFunction
        m_CollisionMatrix[ShapeType::SHAPE_TYPE_COUNT]
                         [ShapeType::SHAPE_TYPE_COUNT] = {
                             {CollideCircleCircle, CollideCircleRect},
                             {CollideRectCircle, CollideRectRect}};
};
}  // namespace Breakout
