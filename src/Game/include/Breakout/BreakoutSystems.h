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

class BreakoutCollisionSystem : public Engine::System {
       public:
        template <uint8_t ComponentCount, uint32_t EntityCount>
        void OnUpdate(Engine::ECS<ComponentCount, EntityCount>& system) {
                for (auto iteratorA = m_Entities.begin(); iteratorA != m_Entities.end(); iteratorA++) {
                        const Engine::Entity entity = *iteratorA;
                        Transform& transform = system.template GetComponent<Transform>(entity);
                        RigidBody& rigidBody = system.template GetComponent<RigidBody>(entity);
                        CollisionShape& shape = system.template GetComponent<CollisionShape>(entity);

                        auto iteratorB = iteratorA;
                        iteratorB++;  // Avoid checking for collisions with itself.

                        for (; iteratorB != m_Entities.end(); iteratorB++) {
                                const Engine::Entity other = *iteratorB;

                                Transform& otherTransform = system.template GetComponent<Transform>(other);
                                RigidBody& otherRigidBody = system.template GetComponent<RigidBody>(other);
                                CollisionShape& otherShape = system.template GetComponent<CollisionShape>(other);

                                // Check collisions.

                                if (shape.type == otherShape.type)
                                        continue;  // Due to the nature of the
                                                   // game, this does nothing.

                                if ((shape.type == ShapeType::BoxCollider &&
                                     otherShape.type == ShapeType::PaddleCollider) ||
                                    (otherShape.type == ShapeType::BoxCollider &&
                                     shape.type == ShapeType::PaddleCollider))
                                        continue;

                                auto getTopDist = [](sf::Vector2f circleLocation, float verticalVal, float radius) {
                                        return verticalVal - circleLocation.y - radius;
                                };

                                auto getButtomDist = [](sf::Vector2f circleLocation, float verticalVal, float radius) {
                                        return circleLocation.y - verticalVal - radius;
                                };

                                auto getLeftDist = [](sf::Vector2f circleLocation, float horizontalVal, float radius) {
                                        return horizontalVal - circleLocation.x - radius;
                                };

                                auto getRightDist = [](sf::Vector2f circleLocation, float horizontalVal, float radius) {
                                        return circleLocation.x - horizontalVal - radius;
                                };

                                // The way it is calculated, they are all box colliders.
                                // This should mabey be changed.
                                if (shape.type == ShapeType::CircleCollider &&
                                    otherShape.type == ShapeType::BoxCollider) {
                                        float radius = shape.shapeData.circle.radius;

                                        float topDistance = getTopDist(
                                            transform.location,
                                            otherTransform.location.y - otherShape.shapeData.box.height / 2.0f, radius);

                                        float buttomDistance = getButtomDist(
                                            transform.location,
                                            otherTransform.location.y + otherShape.shapeData.box.height / 2.0f, radius);

                                        float leftDistance = getLeftDist(
                                            transform.location,
                                            otherTransform.location.x - otherShape.shapeData.box.width / 2.0f, radius);

                                        float rightDistance = getRightDist(
                                            transform.location,
                                            otherTransform.location.x + otherShape.shapeData.box.width / 2.0f, radius);

                                        bool horizontalInside = (leftDistance < 0 && rightDistance < 0);
                                        bool verticalInside = (topDistance < 0 && buttomDistance < 0);

                                        // If both are negative, we know we have a collision.
                                        // If they are not all negative, there is not a collision.
                                        if (horizontalInside && verticalInside) {
                                                // We know that both are negative, so the larger of the two is the
                                                // smallest abs value, and is the one closer to the boundary.
                                                float horizontalDist = leftDistance * (leftDistance > rightDistance) +
                                                                       rightDistance * (rightDistance >= leftDistance);
                                                float verticalDist = topDistance * (topDistance > buttomDistance) +
                                                                     buttomDistance * (buttomDistance >= topDistance);

                                                std::cout << "Denug Print from BreakoutSystems.h, line 157" << "\n";
                                                std::cout << "Horizontal Dist: " << horizontalDist
                                                          << "Vertical Dist: " << verticalDist << "\n";

                                                // If we are closer to the vertical edge, we assume the collision
                                                // happened there, and vice versa.
                                                rigidBody.velocity.x *= 1 - 2 * (horizontalDist > verticalDist);
                                                rigidBody.velocity.y *= 1 - 2 * (verticalDist > horizontalDist);

                                                // Places the ball at the edge of the edge it collided with.
                                                // This avoid the ball getting stuck in the wall, bouncing constantly.
                                                transform.location.x +=
                                                    (leftDistance * (leftDistance > rightDistance) -
                                                     rightDistance * (rightDistance >= leftDistance)) *
                                                    (horizontalDist > verticalDist);

                                                transform.location.y +=
                                                    (topDistance * (topDistance > buttomDistance) -
                                                     buttomDistance * (buttomDistance >= topDistance)) *
                                                    (verticalDist > horizontalDist);
                                        }
                                }

                                if (shape.type == ShapeType::BoxCollider &&
                                    otherShape.type == ShapeType::CircleCollider) {
                                        float radius = otherShape.shapeData.circle.radius;

                                        float topDistance = getTopDist(
                                            otherTransform.location,
                                            transform.location.y - shape.shapeData.box.height / 2.0f, radius);

                                        float buttomDistance = getButtomDist(
                                            otherTransform.location,
                                            transform.location.y + shape.shapeData.box.height / 2.0f, radius);

                                        float leftDistance = getLeftDist(
                                            otherTransform.location,
                                            transform.location.x - shape.shapeData.box.width / 2.0f, radius);

                                        float rightDistance = getRightDist(
                                            otherTransform.location,
                                            transform.location.x + shape.shapeData.box.width / 2.0f, radius);

                                        bool horizontalInside = (leftDistance < 0 && rightDistance < 0);
                                        bool verticalInside = (topDistance < 0 && buttomDistance < 0);

                                        // If both are negative, we know we have a collision.
                                        // If they are not all negative, there is not a collision.
                                        if (horizontalInside && verticalInside) {
                                                // We know that both are negative, so the larger of the two is the
                                                // smallest abs value, and is the one closer to the boundary.
                                                float horizontalDist = leftDistance * (leftDistance > rightDistance) +
                                                                       rightDistance * (rightDistance > leftDistance);
                                                float verticalDist = topDistance * (topDistance > buttomDistance) +
                                                                     buttomDistance * (buttomDistance > topDistance);

                                                // If we are closer to the vertical edge, we assume the collision
                                                // happened there, and vice versa.
                                                otherRigidBody.velocity.x *= 1 - 2 * (horizontalDist > verticalDist);
                                                otherRigidBody.velocity.y *= 1 - 2 * (verticalDist > horizontalDist);

                                                // Places the ball at the edge of the edge it collided with.
                                                // This avoid the ball getting stuck in the wall, bouncing constantly.
                                                otherTransform.location.x +=
                                                    (leftDistance * (leftDistance > rightDistance) -
                                                     rightDistance * (rightDistance >= leftDistance)) *
                                                    (horizontalDist > verticalDist);

                                                otherTransform.location.y +=
                                                    (topDistance * (topDistance > buttomDistance) -
                                                     buttomDistance * (buttomDistance >= topDistance)) *
                                                    (verticalDist > horizontalDist);
                                        }
                                }

                                if (shape.type == ShapeType::CircleCollider &&
                                    otherShape.type == ShapeType::PaddleCollider) {
                                        // Handle this collision.
                                        // For the game, its only really the ball(s) that should be updated, so the
                                        // other can stay static.
                                }

                                if (shape.type == ShapeType::PaddleCollider &&
                                    otherShape.type == ShapeType::CircleCollider) {
                                        // Handle this collision.
                                        // For the game, its only really the ball(s) that should be updated, so the
                                        // other can stay static.
                                }

                                // We skip the Paddle-Paddle, Paddle-Box, Box-Paddle and Box-Box due to the nature of
                                // the game.
                        }
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

class BreakoutPaddleMovementSystem : public Engine::System {
       public:
        template <uint8_t ComponentCount, uint32_t EntityCount>
        void OnUpdate(float deltaTime, Engine::ECS<ComponentCount, EntityCount>& system) {
                for (auto const entity : m_Entities) {
                        Transform& transform = system.template GetComponent<Transform>(entity);
                        HorizontalMovement& horizontalMovement =
                            system.template GetComponent<HorizontalMovement>(entity);

                        bool moveLeft = false;
                        bool moveRight = false;

                        for (const auto key : horizontalMovement.moveLeftKeys) {
                                moveLeft = (moveLeft || sf::Keyboard::isKeyPressed(key));
                        }
                        for (const auto key : horizontalMovement.moveRightKeys) {
                                moveRight = (moveRight || sf::Keyboard::isKeyPressed(key));
                        }

                        transform.location.x += (moveRight - moveLeft) * horizontalMovement.movementSpeed * deltaTime;
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
                signature.set(system.template GetComponentType<HorizontalMovement>(), true);
                signature.set(system.template GetComponentType<Transform>(), true);

                system.template RegisterSystem<BreakoutPaddleMovementSystem>();
                system.template SetSystemSignature<BreakoutPaddleMovementSystem>(signature);
        }
};

class BreakoutRenderSpritesSystem : public Engine::System {
       public:
        template <uint8_t ComponentCount, uint32_t EntityCount>
        void OnRender(Engine::ECS<ComponentCount, EntityCount>& system) {
                for (auto const entity : m_Entities) {
                        Sprite& sprite = system.template GetComponent<Sprite>(entity);
                        Engine::Renderer::Get().Draw(sprite.sprite.GetSprite());

                        sf::Vector2f spriteLocation = sprite.sprite.GetSprite().getOrigin();
                        sf::Vector2f spriteSize = sprite.sprite.GetSprite().getGlobalBounds().size;

                        std::cout << "Debug pring in BreakoutSystems.h in line 323" << "\n";
                        std::cout << "Sprite location: " << spriteLocation.x << spriteLocation.y
                                  << "\n SpriteSize: " << spriteSize.x << spriteSize.y << "\n";
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
