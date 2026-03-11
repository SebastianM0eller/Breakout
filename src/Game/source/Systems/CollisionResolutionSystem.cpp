#include "Systems/CollisionResolutionSystem.h"

#include "BreakoutECS.h"
#include "Components.h"
#include "Engine/ECS/Entity.h"
#include "Events.h"
#include "SFML/System/Vector2.hpp"

void Breakout::CollisionResolutionSystem::BasicBounce(const Engine::Entity entity, const CollisionResult& hit,
                                                      BreakoutECS& system) {
        // We get the data.
        Transform& transform = system.GetComponent<Transform>(entity);
        RigidBody& rigidBody = system.GetComponent<RigidBody>(entity);

        // We correct for the penetration.
        transform.location += hit.normal * hit.penetrationDepth;

        // For find the projection of the velocity onto the normal.
        sf::Vector2f projection = rigidBody.velocity.projectedOnto(hit.normal);

        // Now we flip the normal velocity, by subtracting the projection twice.
        rigidBody.velocity -= projection * 2.0f;
}

void Breakout::CollisionResolutionSystem::PaddleBounce(const Engine::Entity entity, const CollisionResult& hit,
                                                       BreakoutECS& system) {
        // We get the data.
        Transform& transform = system.GetComponent<Transform>(entity);
        RigidBody& rigidBody = system.GetComponent<RigidBody>(entity);
        Transform& transformOther = system.GetComponent<Transform>(hit.other);

        // We correct for the peneration.
        transform.location += hit.normal * hit.penetrationDepth;

        // We update the velocity based on location.
        sf::Vector2f direction = (transform.location - transformOther.location);
        float velocity = rigidBody.velocity.length();
        rigidBody.velocity = velocity * direction.normalized();
}

void Breakout::CollisionResolutionSystem::KillBounce(const Engine::Entity entity, const CollisionResult&,
                                                     BreakoutECS& system) {
        system.AddComponent(entity, Destroyed{});
        system.SendEvent(BallDestroyedEvent{});
}

void Breakout::CollisionResolutionSystem::IgnoreBounce(const Engine::Entity, const CollisionResult&, BreakoutECS&) {}
