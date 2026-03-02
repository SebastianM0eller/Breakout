#pragma once
#include <cstdint>

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

        // Todo: Remove the radius from the lambdas, and just check if the distance squared are larger than the
        // radius squared to see if they collide. (That should make it work correctly)
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

                                // We skip the Paddle-Paddle, Paddle-Box, Box-Paddle and Box-Box due to the nature of
                                // the game.
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

                                                // Correction for the circle hitting the corner of the box.
                                                if (horizontalDist * horizontalDist + verticalDist * verticalDist <
                                                    radius * radius * 0.5)  // The 0.5 is from trial and error.
                                                        continue;

                                                // Closer to the top, and moving down.
                                                bool topVelChange =
                                                    (topDistance > buttomDistance) && (rigidBody.velocity.y > 0);

                                                // Closer to the buttom, and moving up.
                                                bool butVelChange =
                                                    (topDistance < buttomDistance) && (rigidBody.velocity.y < 0);

                                                // Closer to the left, and moving to the right.
                                                bool leftVelChange =
                                                    (leftDistance > rightDistance) && (rigidBody.velocity.x > 0);

                                                // Closer to the right, and moving to the left.
                                                bool rightVelChange =
                                                    (leftDistance < rightDistance) && (rigidBody.velocity.x < 0);

                                                bool horizontalVelChange = (horizontalDist > verticalDist);
                                                bool verticalVelChange = (verticalDist > horizontalDist);

                                                rigidBody.velocity.x *=
                                                    1 - 2 * ((leftVelChange || rightVelChange) && horizontalVelChange);
                                                rigidBody.velocity.y *=
                                                    1 - 2 * ((topVelChange || butVelChange) && verticalVelChange);
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

                                                // Correction for the circle hitting the corner of the box.
                                                if (horizontalDist * horizontalDist + verticalDist * verticalDist <
                                                    radius * radius * 0.5)  // The 0.5 is from trial and error.
                                                        continue;

                                                // Closer to the top, and moving down.
                                                bool topVelChange =
                                                    (topDistance > buttomDistance) && (otherRigidBody.velocity.y > 0);

                                                // Closer to the buttom, and moving up.
                                                bool butVelChange =
                                                    (topDistance < buttomDistance) && (otherRigidBody.velocity.y < 0);

                                                // Closer to the left, and moving to the right.
                                                bool leftVelChange =
                                                    (leftDistance > rightDistance) && (otherRigidBody.velocity.x > 0);

                                                // Closer to the right, and moving to the left.
                                                bool rightVelChange =
                                                    (leftDistance < rightDistance) && (otherRigidBody.velocity.x < 0);

                                                bool horizontalVelChange = (horizontalDist > verticalDist);
                                                bool verticalVelChange = (verticalDist > horizontalDist);

                                                otherRigidBody.velocity.x *=
                                                    1 - 2 * ((leftVelChange || rightVelChange) && horizontalVelChange);
                                                otherRigidBody.velocity.y *=
                                                    1 - 2 * ((topVelChange || butVelChange) && verticalVelChange);
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
