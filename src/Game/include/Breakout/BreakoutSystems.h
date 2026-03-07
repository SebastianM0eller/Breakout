#pragma once
#include <cstdint>
#include <iostream>

#include "Breakout/BreakoutComponents.h"
#include "Engine/ECS/ECS.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/System.h"
#include "Engine/Renderer.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"

class BreakoutPhysicsSystem : public Engine::System {
       public:
        template <uint8_t ComponentCount, uint32_t EntityCount>
        void OnUpdate(float deltaTime, Engine::ECS<ComponentCount, EntityCount>& system) {
                for (auto const entity : m_Entities) {
                        RigidBody& rigidBody = system.template GetComponent<RigidBody>(entity);
                        Transform& transform = system.template GetComponent<Transform>(entity);

                        transform.location.x += rigidBody.velocity.x * deltaTime;
                        transform.location.y += rigidBody.velocity.y * deltaTime;
                }
        }

        ///
        /// Used to easily register the system.
        /// This is only safe to use if the individual Components are already registered.
        /// If they are not, it may result in undefined behaviour.
        ///
        template <uint8_t ComponentCount, uint32_t EntityCount>
        static void RegisterSelf(Engine::ECS<ComponentCount, EntityCount>& system) {
                using Signature = std::bitset<ComponentCount>;

                Signature signature;
                signature.set(system.template GetComponentType<RigidBody>(), true);
                signature.set(system.template GetComponentType<Transform>(), true);

                system.template RegisterSystem<BreakoutPhysicsSystem>();
                system.template SetSystemSignature<BreakoutPhysicsSystem>(signature);
        }
};

class BreakoutUpdateEntityLocationSystem : public Engine::System {
       public:
        template <uint8_t ComponentCount, uint32_t EntityCount>
        void OnUpdate(Engine::ECS<ComponentCount, EntityCount>& system) {
                for (auto const entity : m_Entities) {
                        Transform& transform = system.template GetComponent<Transform>(entity);
                        Sprite& sprite = system.template GetComponent<Sprite>(entity);

                        sprite.sprite.GetSprite().setPosition({transform.location.x, transform.location.y});
                }
        }

        ///
        /// Used to easily register the system.
        /// This is only safe to use if the individual Components are already registered.
        /// If they are not, it may result in undefined behaviour.
        ///
        template <uint8_t ComponentCount, uint32_t EntityCount>
        static void RegisterSelf(Engine::ECS<ComponentCount, EntityCount>& system) {
                using Signature = std::bitset<ComponentCount>;

                Signature signature;
                signature.set(system.template GetComponentType<Transform>(), true);
                signature.set(system.template GetComponentType<Sprite>(), true);

                system.template RegisterSystem<BreakoutUpdateEntityLocationSystem>();
                system.template SetSystemSignature<BreakoutUpdateEntityLocationSystem>(signature);
        }
};

class BreakoutRenderSpritesSystem : public Engine::System {
       public:
        template <uint8_t ComponentCount, uint32_t EntityCount>
        void OnRender(Engine::ECS<ComponentCount, EntityCount>& system) {
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
        template <uint8_t ComponentCount, uint32_t EntityCount>
        static void RegisterSelf(Engine::ECS<ComponentCount, EntityCount>& system) {
                using Signature = std::bitset<ComponentCount>;

                Signature signature;
                signature.set(system.template GetComponentType<Sprite>(), true);

                system.template RegisterSystem<BreakoutRenderSpritesSystem>();
                system.template SetSystemSignature<BreakoutRenderSpritesSystem>(signature);
        }
};

class PlayerMovementSystem : public Engine::System {
       public:
        template <uint8_t ComponentCount, uint32_t EntityCount>
        void OnUpdate(Engine::ECS<ComponentCount, EntityCount>& system) {
                for (auto const entity : m_Entities) {
                        RigidBody& rigidBody = system.template GetComponent<RigidBody>(entity);
                        Transform& transform = system.template GetComponent<Transform>(entity);
                        CollisionShape& shape = system.template GetComponent<CollisionShape>(entity);

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
                        float paddleSize = shape.shapeData.box.width;

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
        template <uint8_t ComponentCount, uint32_t EntityCount>
        static void RegisterSelf(Engine::ECS<ComponentCount, EntityCount>& system) {
                using Signature = std::bitset<ComponentCount>;

                Signature signature;
                signature.set(system.template GetComponentType<Player>(), true);
                signature.set(system.template GetComponentType<RigidBody>(), true);

                system.template RegisterSystem<PlayerMovementSystem>();
                system.template SetSystemSignature<PlayerMovementSystem>(signature);
        }
};
