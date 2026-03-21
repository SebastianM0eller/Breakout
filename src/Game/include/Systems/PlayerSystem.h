#pragma once

#include <Engine/ECS/System.h>
#include <Engine/Renderer.h>

#include <SFML/Window/Keyboard.hpp>

#include "BreakoutECS.h"
#include "Components.h"
#include "Events.h"

namespace Breakout {
class PlayerSystem : public Engine::System {
   public:
    void OnUpdate(BreakoutECS& system) {
        for (auto const entity : m_Entities) {
            RigidBody& rigidBody = system.GetComponent<RigidBody>(entity);
            Transform& transform = system.GetComponent<Transform>(entity);
            ColliderComponent& shape =
                system.GetComponent<ColliderComponent>(entity);

            constexpr float speed = 250;

            // Reset the velocity of the player.
            rigidBody.velocity.x = 0;
            rigidBody.velocity.y = 0;

            rigidBody.velocity.x +=
                speed * (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
                         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right));

            rigidBody.velocity.x -=
                speed * (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
                         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left));

            // Place the paddle back into the map, if it is leaving it.
            sf::Vector2f viewSize = Engine::Renderer::Get().GetViewSize();
            float paddleSize = shape.rect.width;

            float rightOverlap =
                transform.location.x + paddleSize / 2.0f - viewSize.x;
            transform.location.x -= rightOverlap * (rightOverlap > 0);

            float leftOverlap = transform.location.x - paddleSize / 2.0f;
            transform.location.x -= leftOverlap * (leftOverlap < 0);
        }
    }

    void AddBallSpawner(BreakoutECS& system) {
        for (const Engine::Entity entity : m_Entities) {
            system.AddComponent(entity, AvailableBallSpawn{});
        }
    }

    static void RegisterSelf(BreakoutECS& system) {
        BreakoutSignature signature;
        signature.set(system.GetComponentType<Breakout::Player>(), true);

        auto self = system.RegisterSystem<PlayerSystem>();
        system.SetSystemSignature<PlayerSystem>(signature);

        // Register the callbacks using a weak_ptr for lifetime.
        std::weak_ptr<PlayerSystem> weakSelf = self;

        system.AddListner<SpawnBallEvent>(
            [weakSelf, &system](const SpawnBallEvent&) {
                if (auto lockedSelf = weakSelf.lock()) {
                    lockedSelf->AddBallSpawner(system);
                }
            });
    }
};
}  // namespace Breakout
