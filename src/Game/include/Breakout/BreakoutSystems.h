#pragma once
#include <cstdint>

#include "Breakout/BreakoutComponents.h"
#include "Engine/ECS/ECS.h"
#include "Engine/ECS/System.h"
#include "Engine/Renderer.h"

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

class BreakoutCollisionSystem : public Engine::System {
       public:
        template <uint8_t ComponentCount, uint32_t EntityCount>
        void OnUpdate(Engine::ECS<ComponentCount, EntityCount>& system) {
                for (auto const entity : m_Entities) {
                        Transform& transform = system.template GetComponent<Transform>(entity);
                        RigidBody& rigidBody = system.template GetComponent<RigidBody>(entity);
                        // For the simple test, i dont care for the collision.

                        if (transform.location.x < 0 && rigidBody.velocity.x < 0) rigidBody.velocity.x *= -1;
                        if (transform.location.x > 800 && rigidBody.velocity.x > 0) rigidBody.velocity.x *= -1;
                        if (transform.location.y < 0 && rigidBody.velocity.y < 0) rigidBody.velocity.y *= -1;
                        if (transform.location.y > 600 && rigidBody.velocity.y > 0) rigidBody.velocity.y *= -1;
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
                signature.set(system.template GetComponentType<RigidBody>(), true);
                signature.set(system.template GetComponentType<CollisionShape>(), true);

                system.template RegisterSystem<BreakoutCollisionSystem>();
                system.template SetSystemSignature<BreakoutCollisionSystem>(signature);
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
