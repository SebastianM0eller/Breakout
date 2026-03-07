#pragma once

#include "Breakout/BreakoutComponents.h"
#include "BreakoutECS.h"
#include "Components.h"
#include "Engine/ECS/ECS.h"
#include "Engine/ECS/System.h"
#include "Engine/Renderer.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"

class BreakoutPhysicsSystem : public Engine::System {
       public:
        void OnUpdate(float deltaTime, BreakoutECS& system) {
                for (auto const entity : m_Entities) {
                        RigidBody& rigidBody = system.GetComponent<RigidBody>(entity);
                        Breakout::Transform& transform = system.GetComponent<Breakout::Transform>(entity);

                        transform.location.x += rigidBody.velocity.x * deltaTime;
                        transform.location.y += rigidBody.velocity.y * deltaTime;
                }
        }

        ///
        /// Used to easily register the system.
        /// This is only safe to use if the individual Components are already registered.
        /// If they are not, it may result in undefined behaviour.
        ///
        static void RegisterSelf(BreakoutECS& system) {
                BreakoutSignature signature;
                signature.set(system.GetComponentType<RigidBody>(), true);
                signature.set(system.GetComponentType<Breakout::Transform>(), true);

                system.RegisterSystem<BreakoutPhysicsSystem>();
                system.template SetSystemSignature<BreakoutPhysicsSystem>(signature);
        }
};

class BreakoutUpdateEntityLocationSystem : public Engine::System {
       public:
        void OnUpdate(BreakoutECS& system) {
                for (auto const entity : m_Entities) {
                        Breakout::Transform& transform = system.GetComponent<Breakout::Transform>(entity);
                        Sprite& sprite = system.GetComponent<Sprite>(entity);

                        sprite.sprite.GetSprite().setPosition({transform.location.x, transform.location.y});
                }
        }

        ///
        /// Used to easily register the system.
        /// This is only safe to use if the individual Components are already registered.
        /// If they are not, it may result in undefined behaviour.
        ///
        static void RegisterSelf(BreakoutECS& system) {
                BreakoutSignature signature;
                signature.set(system.GetComponentType<Breakout::Transform>(), true);
                signature.set(system.GetComponentType<Sprite>(), true);

                system.RegisterSystem<BreakoutUpdateEntityLocationSystem>();
                system.SetSystemSignature<BreakoutUpdateEntityLocationSystem>(signature);
        }
};

class BreakoutRenderSpritesSystem : public Engine::System {
       public:
        void OnRender(BreakoutECS& system) {
                for (auto const entity : m_Entities) {
                        Sprite& sprite = system.template GetComponent<Sprite>(entity);
                        Engine::Renderer::Get().Draw(sprite.sprite.GetSprite());
                }
        }
        ///
        /// Used to easily register the system.
        /// This is only safe to use if the individual Components are already registered.
        /// If they are not, it may result in undefined behaviour.
        ///
        static void RegisterSelf(BreakoutECS& system) {
                BreakoutSignature signature;
                signature.set(system.GetComponentType<Sprite>(), true);

                system.RegisterSystem<BreakoutRenderSpritesSystem>();
                system.SetSystemSignature<BreakoutRenderSpritesSystem>(signature);
        }
};

class PlayerMovementSystem : public Engine::System {
       public:
        void OnUpdate(BreakoutECS& system) {
                for (auto const entity : m_Entities) {
                        RigidBody& rigidBody = system.GetComponent<RigidBody>(entity);
                        Breakout::Transform& transform = system.GetComponent<Breakout::Transform>(entity);
                        Breakout::ColliderComponent& shape = system.GetComponent<Breakout::ColliderComponent>(entity);

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

        ///
        /// Used to easily register the system.
        /// This is only safe to use if the individual Components are already registered.
        /// If they are not, it may result in undefined behaviour.
        ///
        static void RegisterSelf(BreakoutECS& system) {
                BreakoutSignature signature;
                signature.set(system.GetComponentType<Player>(), true);
                signature.set(system.GetComponentType<RigidBody>(), true);

                system.RegisterSystem<PlayerMovementSystem>();
                system.SetSystemSignature<PlayerMovementSystem>(signature);
        }
};
