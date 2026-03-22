#include "Systems/CollisionResolutionSystem.h"

#include <cstdlib>

#include "BreakoutECS.h"
#include "Components.h"
#include "Engine/ECS/Entity.h"
#include "Events.h"
#include "SFML/System/Vector2.hpp"

void Breakout::CollisionResolutionSystem::BasicBounce(
    const Engine::Entity entity, const CollisionResult& hit,
    BreakoutECS& system) {
    // We get the data.
    Transform& transform = system.GetComponent<Transform>(entity);
    RigidBody& rigidBody = system.GetComponent<RigidBody>(entity);

    // We adjust the normal, to be 0, 90, 180 or 270 degrees.
    sf::Vector2f newNormal;

    if (std::abs(hit.normal.x) > std::abs(hit.normal.y)) {
        newNormal.x = hit.normal.x;
        newNormal.y = 0;
    } else {
        newNormal.x = 0;
        newNormal.y = hit.normal.y;
    }

    // We correct for the penetration, using the old normal.
    transform.location += hit.normal * hit.penetrationDepth;

    // For find the projection of the velocity onto the normal, using the new
    // normal.
    sf::Vector2f projection = rigidBody.velocity.projectedOnto(newNormal);

    // We check if we are moving into the object. This is to avoid the velocity
    // being flipped twice, if we hit more than one object a frame.
    if (projection.dot(newNormal) < 0) {
        // Now we flip the normal velocity, by subtracting the projection twice.
        rigidBody.velocity -= (projection) * 2.0f;
    }
}

void Breakout::CollisionResolutionSystem::RegularBounce(
    const Engine::Entity entity, const CollisionResult& hit,
    BreakoutECS& system) {
    // We get the data.
    Transform& transform = system.GetComponent<Transform>(entity);
    RigidBody& rigidBody = system.GetComponent<RigidBody>(entity);

    // We correct for the penetration.
    transform.location += hit.normal * hit.penetrationDepth;

    // For find the projection of the velocity onto the normal.
    sf::Vector2f projection = rigidBody.velocity.projectedOnto(hit.normal);

    // We check if we are moving into the object. This is to avoid the velocity
    // being flipped twice, if we hit more than one object a frame.
    if (projection.dot(hit.normal) < 0) {
        // Now we flip the normal velocity, by subtracting the projection twice.
        rigidBody.velocity -= (projection) * 2.0f;
    }
}

void Breakout::CollisionResolutionSystem::PaddleBounce(
    const Engine::Entity entity, const CollisionResult& hit,
    BreakoutECS& system) {
    assert(system.HasComponent<Ball>(entity) &&
           "Only the ball, should collide with the paddle.");

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

    // We reset the multiplier for the ball.
    uint32_t& multiplier = system.GetComponent<Ball>(entity).scoreIncrease;
    multiplier = 0;
}

void Breakout::CollisionResolutionSystem::KillBoxBounce(
    const Engine::Entity entity, const CollisionResult& hit,
    BreakoutECS& system) {
    assert(system.HasComponent<Ball>(hit.other) &&
           "A box should only be destroyed, if it collides with a ball");

    uint32_t& increase = system.GetComponent<Ball>(hit.other).scoreIncrease;

    system.SendEvent(ScoreIncreasedEvent{25 + increase});
    system.SendEvent(BoxDestroyedEvent{});
    system.AddComponent(entity, Destroyed{});

    increase += 5;
}

void Breakout::CollisionResolutionSystem::KillBounce(
    const Engine::Entity entity, const CollisionResult&, BreakoutECS& system) {
    system.AddComponent(entity, Destroyed{});
    system.SendEvent(BallDestroyedEvent{});
}

void Breakout::CollisionResolutionSystem::IgnoreBounce(const Engine::Entity,
                                                       const CollisionResult&,
                                                       BreakoutECS&) {
}
