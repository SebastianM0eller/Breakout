#include "Systems/BallLifeSystem.h"

#include <Engine/ECS/Entity.h>

#include <cassert>
#include <cstdint>

#include "Components.h"
#include "Events.h"

void Breakout::BallLifeSystem::LifeGained(BreakoutECS& system) {
        uint32_t first_available_entity = 0;          // To track the entity id.
        float first_available_location = 1000000.0f;  // To track what entity to 'revive'
        bool availableLifeIncrease = false;           // Track if we are at max lifes.

        for (const Engine::Entity entity : m_Entities) {
                BallSlots& slot = system.GetComponent<BallSlots>(entity);
                Transform& transform = system.GetComponent<Transform>(entity);

                availableLifeIncrease = (availableLifeIncrease || slot.empty);

                if (slot.empty && transform.location.x < first_available_location) {
                        first_available_entity = entity;
                        first_available_location = transform.location.x;
                }
        }

        if (availableLifeIncrease) {
                // Update the entity.
                BallSlots& slot = system.GetComponent<BallSlots>(first_available_entity);
                slot.empty = false;

                Sprite& sprite = system.GetComponent<Sprite>(first_available_entity);
                sprite.sprite = Engine::ManagedSprite("assets/Textures/AvailableBall.png");
        }
}

void Breakout::BallLifeSystem::LifeUsed(BreakoutECS& system) {
        uint32_t first_available_entity = 0;          // To track the entity id.
        float first_available_location = 1000000.0f;  // To track what entity to 'kill'.
        bool AvailableLifeDecrease = false;           // Should never be false, as the game would be over otherwise.

        for (const Engine::Entity entity : m_Entities) {
                BallSlots& slot = system.GetComponent<BallSlots>(entity);
                Transform& transform = system.GetComponent<Transform>(entity);

                AvailableLifeDecrease = (AvailableLifeDecrease || !slot.empty);

                if (!slot.empty && transform.location.x < first_available_location) {
                        first_available_entity = entity;
                        first_available_location = transform.location.x;
                }
        }

        if (AvailableLifeDecrease) {
                // Update the entity
                BallSlots& slot = system.GetComponent<BallSlots>(first_available_entity);
                slot.empty = true;

                Sprite& sprite = system.GetComponent<Sprite>(first_available_entity);
                sprite.sprite = Engine::ManagedSprite("assets/Textures/UsedBall.png");

                // If we have more lives, we should spawn a new ball.
                system.SendEvent(SpawnBall{});
        } else {
                // If we have no more lives to lose, the game must be over.
                system.SendEvent(GameOverEvent{});
        }
}
