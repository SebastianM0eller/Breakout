#include "Systems/CollisionDetectionSystem.h"

#include <algorithm>
#include <cmath>

#include "Components.h"
#include "SFML/System/Vector2.hpp"

Breakout::CollisionResult
Breakout::CollisionDetectionSystem::CollideCircleCircle(
    const Breakout::WorldCollider& a, const Breakout::WorldCollider& b) {
    // Get the vector from b to a.
    sf::Vector2f difference = a.transform.location - b.transform.location;

    float DifferenceDistanceSQ = difference.lengthSquared();
    float radiiSum = a.shape.circle.radius + b.shape.circle.radius;
    float radiiSumSQ = radiiSum * radiiSum;

    // Early exit, check if they are not overlapping.
    if (DifferenceDistanceSQ > radiiSumSQ)
        return {{0.0f, 0.0f}, 0, 0.0f, false};

    // We now know we are colliding.

    float distance = std::sqrt(DifferenceDistanceSQ);
    float depth = radiiSum - distance;
    sf::Vector2f normal = difference / distance;

    return {normal, 0, depth, true};
}

Breakout::CollisionResult Breakout::CollisionDetectionSystem::CollideCircleRect(
    const Breakout::WorldCollider& a, const Breakout::WorldCollider& b) {
    // Get the vector from the rect to the circle.
    sf::Vector2f difference = a.transform.location - b.transform.location;

    float halfWidth = b.shape.rect.width / 2.0f;
    float halfHeight = b.shape.rect.height / 2.0f;

    // Get the closest point on the rect to the circle.
    sf::Vector2f clamped;
    clamped.x = std::clamp(difference.x, -halfWidth, halfWidth);
    clamped.y = std::clamp(difference.y, -halfHeight, halfHeight);

    sf::Vector2f closestPoint = b.transform.location + clamped;

    // Update the difference based on the closest point.
    difference = a.transform.location - closestPoint;

    float distanceSQ = difference.lengthSquared();
    float radius = a.shape.circle.radius;
    float radiusSQ = radius * radius;

    // Early exit, check if the closes point is not within the circle.
    if (distanceSQ > radiusSQ)
        return {{0.0f, 0.0f}, 0, 0.0f, false};

    // We now know we are colliding.

    float distance = std::sqrt(distanceSQ);
    float depth = radius - distance;
    sf::Vector2f normal = difference / distance;

    return {normal, 0, depth, true};
}

Breakout::CollisionResult Breakout::CollisionDetectionSystem::CollideRectCircle(
    const Breakout::WorldCollider& a, const Breakout::WorldCollider& b) {
    // Get the result.
    CollisionResult result = CollideCircleRect(b, a);

    // Reverse the normal, to correct for the reverse order.
    result.normal = -result.normal;

    return result;
}

Breakout::CollisionResult Breakout::CollisionDetectionSystem::CollideRectRect(
    const Breakout::WorldCollider& a, const Breakout::WorldCollider& b) {
    // Get the vector from b to a.
    sf::Vector2f difference = a.transform.location - b.transform.location;

    float aHalfWidth = a.shape.rect.width / 2.0f;
    float aHalfHeight = a.shape.rect.height / 2.0f;

    float bHalfWidth = b.shape.rect.width / 2.0f;
    float bHalfHeight = b.shape.rect.height / 2.0f;

    float xOverlap = (aHalfWidth + bHalfWidth) - std::abs(difference.x);
    float yOverlap = (aHalfHeight + bHalfHeight) - std::abs(difference.y);

    // Early exit, check if they overlap.
    if (xOverlap <= 0 || yOverlap <= 0)
        return {{0.0f, 0.0f}, 0, 0.0f, false};

    // We now know we are colliding.
    if (xOverlap < yOverlap)
        return (difference.x > 0.0f)
                   ? Breakout::CollisionResult({1.0f, 0.0f}, 0, xOverlap, true)
                   : Breakout::CollisionResult({-1.0f, 0.0f}, 0, xOverlap,
                                               true);

    else
        return (difference.y > 0.0f)
                   ? Breakout::CollisionResult{{0.0f, 1.0f}, 0, yOverlap, true}
                   : Breakout::CollisionResult{
                         {0.0f, -1.0f}, 0, yOverlap, true};
}
