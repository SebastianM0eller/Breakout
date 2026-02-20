#pragma once

#include <cstdint>
#include <iostream>

#include "Engine/ECS/ECS.h"
#include "Engine/ECS/System.h"
#include "Example/ExampleStructs.h"

class PhysicsSystem : public Engine::System {
       public:
        template <uint8_t ComponentCount, uint32_t EntityCount>
        void OnUpdate(float deltaTime, Engine::ECS<ComponentCount, EntityCount>& system) {
                for (auto const& entity : m_Entities) {
                        auto& rigidBody = system.template GetComponent<RigidBody>(entity);
                        auto& location = system.template GetComponent<Location>(entity);

                        location.location += rigidBody.velocity * deltaTime;
                        rigidBody.velocity += rigidBody.acceleration * deltaTime;
                };
        }
};

class LocationLoggingSystem : public Engine::System {
       public:
        template <uint8_t ComponentCount, uint32_t EntityCount>
        void OnUpdate(Engine::ECS<ComponentCount, EntityCount>& system) {
                for (auto const& entity : m_Entities) {
                        auto location = system.template GetComponent<Location>(entity);

                        std::cout << "The position of the entity is: {" << location.location.x << ", "
                                  << location.location.y << "}\n";
                }
        }
};
