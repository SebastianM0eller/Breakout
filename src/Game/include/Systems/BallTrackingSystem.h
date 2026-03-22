#pragma once

#include <Engine/ECS/System.h>

#include <cmath>

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

    void SyncSpeed(BreakoutECS& system) {
        // Find the ball, with the higest speed.
        float maxSpeed{0};

        for (auto const entity : m_Entities) {
            const RigidBody rigidBody = system.GetComponent<RigidBody>(entity);
            float speed = rigidBody.velocity.lengthSquared();
            maxSpeed += (speed - maxSpeed) * (speed > maxSpeed);
        }

        maxSpeed = std::sqrt(maxSpeed);

        // Set the speed for all the balls.
        for (const auto entity : m_Entities) {
            RigidBody& rigidBody = system.GetComponent<RigidBody>(entity);
            rigidBody.velocity = rigidBody.velocity.normalized() * maxSpeed;
        }
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
    bool m_CheckBallAmount{true};
};
}  // namespace Breakout
