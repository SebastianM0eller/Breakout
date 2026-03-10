#include "BreakoutGameLayer.h"

// Systems
#include "Components.h"
#include "Entities.h"
#include "Systems/BallLifeSystem.h"
#include "Systems/BallTrackingSystem.h"
#include "Systems/CollisionDetectionSystem.h"
#include "Systems/CollisionResolutionSystem.h"
#include "Systems/LocationSyncSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/PlayerMovementSystem.h"
#include "Systems/RenderSystem.h"

BreakoutGameLayer::BreakoutGameLayer() {
        m_ECS.Init();
        RegisterComponents();
        RegisterSystems();
        RegisterEntities();
}

void BreakoutGameLayer::RegisterComponents() {
        m_ECS.RegisterComponent<Breakout::RigidBody>();
        m_ECS.RegisterComponent<Breakout::Transform>();
        m_ECS.RegisterComponent<Breakout::Sprite>();
        m_ECS.RegisterComponent<Breakout::Player>();
        m_ECS.RegisterComponent<Breakout::Ball>();
        m_ECS.RegisterComponent<Breakout::BallSlots>();
        m_ECS.RegisterComponent<Breakout::ColliderComponent>();
        m_ECS.RegisterComponent<Breakout::CollisionEvents>();
}

void BreakoutGameLayer::RegisterSystems() {
        Breakout::PhysicsSystem::RegisterSelf(m_ECS);
        Breakout::LocationSyncSystem::RegisterSelf(m_ECS);
        Breakout::RenderSystem::RegisterSelf(m_ECS);
        Breakout::PlayerMovementSystem::RegisterSelf(m_ECS);
        Breakout::CollisionDetectionSystem::RegisterSelf(m_ECS);
        Breakout::CollisionResolutionSystem::RegisterSelf(m_ECS);
        Breakout::BallTrackingSystem::RegisterSelf(m_ECS);
        Breakout::BallLifeSystem::RegisterSelf(m_ECS);
}

void BreakoutGameLayer::RegisterEntities() {
        sf::Vector2f viewSize = Engine::Renderer::Get().GetViewSize();

        Breakout::Transform ballLocation({viewSize.x / 2.0f, viewSize.y - 60.0f});
        Breakout::RigidBody ballSpeed({0, -200});
        Breakout::RegisterBall(m_ECS, ballLocation, ballSpeed);

        Breakout::Transform paddleLocation({viewSize.x / 2.0f, viewSize.y - 45});
        Breakout::RegisterPaddle(m_ECS, paddleLocation);

        Breakout::RegisterWalls(m_ECS, viewSize);
}

void BreakoutGameLayer::OnUpdate(float deltaTime) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) deltaTime *= 0.1f;
        m_ECS.GetSystem<Breakout::PlayerMovementSystem>()->OnUpdate(m_ECS);
        m_ECS.GetSystem<Breakout::PhysicsSystem>()->OnUpdate(deltaTime, m_ECS);
        m_ECS.GetSystem<Breakout::CollisionDetectionSystem>()->OnUpdate(m_ECS);
        m_ECS.GetSystem<Breakout::CollisionResolutionSystem>()->OnUpdate(m_ECS);
        m_ECS.GetSystem<Breakout::LocationSyncSystem>()->OnUpdate(m_ECS);
}

void BreakoutGameLayer::OnRender() { m_ECS.GetSystem<Breakout::RenderSystem>()->OnRender(m_ECS); }
