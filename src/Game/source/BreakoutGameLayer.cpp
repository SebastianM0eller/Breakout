#include "BreakoutGameLayer.h"

// Systems
#include "Components.h"
#include "Entities.h"
#include "Systems/BallLifeSystem.h"
#include "Systems/BallTrackingSystem.h"
#include "Systems/BoxSpawningSystem.h"
#include "Systems/CollisionDetectionSystem.h"
#include "Systems/CollisionResolutionSystem.h"
#include "Systems/DestroyedSystem.h"
#include "Systems/LerpingSystem.h"
#include "Systems/LocationSyncSystem.h"
#include "Systems/PaddleBallSpawnSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/PlayerSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/ScoreSystem.h"

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
    m_ECS.RegisterComponent<Breakout::Destroyed>();
    m_ECS.RegisterComponent<Breakout::AvailableBallSpawn>();
    m_ECS.RegisterComponent<Breakout::Score>();
    m_ECS.RegisterComponent<Breakout::Text>();
    m_ECS.RegisterComponent<Breakout::Box>();
    m_ECS.RegisterComponent<Breakout::LerpComponent>();
}

void BreakoutGameLayer::RegisterSystems() {
    Breakout::PhysicsSystem::RegisterSelf(m_ECS);
    Breakout::LocationSyncSystem::RegisterSelf(m_ECS);
    Breakout::RenderSystem::RegisterSelf(m_ECS);
    Breakout::PlayerSystem::RegisterSelf(m_ECS);
    Breakout::CollisionDetectionSystem::RegisterSelf(m_ECS);
    Breakout::CollisionResolutionSystem::RegisterSelf(m_ECS);
    Breakout::BallTrackingSystem::RegisterSelf(m_ECS);
    Breakout::BallLifeSystem::RegisterSelf(m_ECS);
    Breakout::DestroyedSystem::RegisterSelf(m_ECS);
    Breakout::PaddleBallSpawnSystem::RegisterSelf(m_ECS);
    Breakout::ScoreSystem::RegisterSelf(m_ECS);
    Breakout::BoxSpawningSystem::RegisterSelf(m_ECS);
    Breakout::LerpingSystem::RegisterSelf(m_ECS);
}

void BreakoutGameLayer::RegisterEntities() {
    sf::Vector2f viewSize = Engine::Renderer::Get().GetViewSize();

    Breakout::Transform paddleLocation({viewSize.x / 2.0f, viewSize.y - 45});
    Breakout::RegisterPaddle(m_ECS, paddleLocation);

    Breakout::RegisterWalls(m_ECS, viewSize);
    Breakout::RegisterLifes(m_ECS);
    Breakout::RegisterScore(m_ECS, viewSize);

    Breakout::RegisterBoxes(m_ECS);
}

void BreakoutGameLayer::OnUpdate(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        deltaTime *= 0.1f;
    m_ECS.GetSystem<Breakout::PlayerSystem>()->OnUpdate(m_ECS);
    m_ECS.GetSystem<Breakout::PhysicsSystem>()->OnUpdate(deltaTime, m_ECS);
    m_ECS.GetSystem<Breakout::LerpingSystem>()->OnUpdate(m_ECS, deltaTime);
    m_ECS.GetSystem<Breakout::CollisionDetectionSystem>()->OnUpdate(m_ECS);
    m_ECS.GetSystem<Breakout::CollisionResolutionSystem>()->OnUpdate(m_ECS);
    m_ECS.GetSystem<Breakout::LocationSyncSystem>()->OnUpdate(m_ECS);
    m_ECS.GetSystem<Breakout::DestroyedSystem>()->OnUpdate(
        m_ECS);  // Should always be after the physics update.
    m_ECS.GetSystem<Breakout::BallTrackingSystem>()->OnUpdate(
        m_ECS);  // Needs to run before the destructive
                 // systems, or after the destruction.
    m_ECS.GetSystem<Breakout::PaddleBallSpawnSystem>()->OnUpdate(m_ECS);
    m_ECS.GetSystem<Breakout::BoxSpawningSystem>()->OnUpdate(m_ECS);
}

void BreakoutGameLayer::OnRender() {
    m_ECS.GetSystem<Breakout::RenderSystem>()->OnRender(m_ECS);
    m_ECS.GetSystem<Breakout::ScoreSystem>()->OnRender(m_ECS);
}
