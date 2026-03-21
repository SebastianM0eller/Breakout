#pragma once

#include <Engine/ECS/System.h>

#include "BreakoutECS.h"
#include "Components.h"
#include "Engine/ECS/Entity.h"
#include "Entities.h"
#include "SFML/Window/Keyboard.hpp"

namespace Breakout {
class PaddleBallSpawnSystem : public Engine::System {
   public:
    void OnUpdate(BreakoutECS& system) {
        if (!m_Entities.empty() &&
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            std::vector<Engine::Entity> AvailableBallSpawnToDestroy;

            for (const Engine::Entity entity : m_Entities) {
                Transform& transform = system.GetComponent<Transform>(entity);

                RigidBody spawnRigidBody({0.0f, -200.0f});
                Transform spawnTransform = transform;
                spawnTransform.location.y -= 20;

                RegisterBall(system, spawnTransform, spawnRigidBody);

                AvailableBallSpawnToDestroy.push_back(
                    entity);  // After is has spawned the ball, it
                              // should not be able to spawn anymore.

                break;  // We only ever want to spawn 1 ball.
            }

            for (const auto entity : AvailableBallSpawnToDestroy) {
                system.RemoveComponent<AvailableBallSpawn>(entity);
            }
        }
    }

    static void RegisterSelf(BreakoutECS& system) {
        BreakoutSignature signature;
        signature.set(system.GetComponentType<AvailableBallSpawn>(), true);
        signature.set(system.GetComponentType<Player>(), true);

        system.RegisterSystem<PaddleBallSpawnSystem>();
        system.SetSystemSignature<PaddleBallSpawnSystem>(signature);
    }

    // Add the callback, but it needs it should be here lol.
};
}  // namespace Breakout
