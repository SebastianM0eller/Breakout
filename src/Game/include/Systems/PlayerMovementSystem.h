#pragma once

#include <Engine/ECS/System.h>
#include <Engine/Renderer.h>

#include <SFML/Window/Keyboard.hpp>

#include "BreakoutECS.h"
#include "Components.h"

namespace Breakout {
class PlayerMovementSystem : public Engine::System {
       public:
        void OnUpdate(BreakoutECS& system) {
                for (auto const entity : m_Entities) {
                        RigidBody& rigidBody = system.GetComponent<RigidBody>(entity);
                        Transform& transform = system.GetComponent<Transform>(entity);
                        ColliderComponent& shape = system.GetComponent<ColliderComponent>(entity);

                        constexpr float speed = 250;

                        // Reset the velocity of the player.
                        rigidBody.velocity.x = 0;
                        rigidBody.velocity.y = 0;

                        rigidBody.velocity.x += speed * (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
                                                         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right));

                        rigidBody.velocity.x -= speed * (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
                                                         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left));

                        // Place the paddle back into the map, if it is leaving it.
                        sf::Vector2f viewSize = Engine::Renderer::Get().GetViewSize();
                        float paddleSize = shape.rect.width;

                        float rightOverlap = transform.location.x + paddleSize / 2.0f - viewSize.x;
                        transform.location.x -= rightOverlap * (rightOverlap > 0);

                        float leftOverlap = transform.location.x - paddleSize / 2.0f;
                        transform.location.x -= leftOverlap * (leftOverlap < 0);
                }
        }

        static void RegisterSelf(BreakoutECS& system) {
                BreakoutSignature signature;
                signature.set(system.GetComponentType<Breakout::Player>(), true);
                signature.set(system.GetComponentType<Breakout::RigidBody>(), true);

                system.RegisterSystem<PlayerMovementSystem>();
                system.SetSystemSignature<PlayerMovementSystem>(signature);
        }
};
}  // namespace Breakout
