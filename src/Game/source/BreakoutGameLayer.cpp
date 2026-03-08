#include "BreakoutGameLayer.h"

// Systems
#include "Components.h"
#include "Entities.h"
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
}

void BreakoutGameLayer::RegisterEntities() {
        sf::Vector2f viewSize = Engine::Renderer::Get().GetViewSize();

        Breakout::Transform ballLocation({viewSize.x / 2.0f, viewSize.y - 60.0f});
        Breakout::RigidBody ballSpeed({0, -200});
        Breakout::RegisterBall(m_ECS, ballLocation, ballSpeed);

        // Add the paddle
        Engine::Entity paddleEntity = m_ECS.CreateEntity();
        m_ECS.AddComponent(paddleEntity, Breakout::Transform({viewSize.x / 2.0f, viewSize.y - 45}));
        m_ECS.AddComponent(paddleEntity,
                           Breakout::Sprite({Engine::ManagedSprite("assets/Textures/BreakoutPaddle.png")}));
        m_ECS.AddComponent(paddleEntity, Breakout::RigidBody({0, 0}));
        m_ECS.AddComponent(paddleEntity, Breakout::Player{});
        m_ECS.AddComponent(paddleEntity, Breakout::CollisionEvents{{}, 0});

        Breakout::ColliderComponent paddleCollider = {
            .rect = {56.0f, 6.0f},
            .type = Breakout::ShapeType::SHAPE_RECTANGLE,
            .tag = Breakout::PhysicsTag::PHYSICS_PADDLE,
        };
        m_ECS.AddComponent(paddleEntity, paddleCollider);

        // Add the walls.
        Engine::Entity topWall = m_ECS.CreateEntity();
        m_ECS.AddComponent(topWall, Breakout::Transform({viewSize.x / 2.0f, 0.1f}));
        m_ECS.AddComponent(topWall, Breakout::RigidBody({0, 0}));
        m_ECS.AddComponent(topWall, Breakout::CollisionEvents{{}, 0});

        Breakout::ColliderComponent topShape = {.rect = {viewSize.x, 0},
                                                .type = Breakout::ShapeType::SHAPE_RECTANGLE,
                                                .tag = Breakout::PhysicsTag::PHYSICS_WALL};
        m_ECS.AddComponent(topWall, topShape);

        Engine::Entity leftWall = m_ECS.CreateEntity();
        m_ECS.AddComponent(leftWall, Breakout::Transform({0.1f, viewSize.y / 2.0f}));
        m_ECS.AddComponent(leftWall, Breakout::RigidBody({0, 0}));
        m_ECS.AddComponent(leftWall, Breakout::CollisionEvents{{}, 0});

        Breakout::ColliderComponent leftShape = {.rect = {0, viewSize.y},
                                                 .type = Breakout::ShapeType::SHAPE_RECTANGLE,
                                                 .tag = Breakout::PhysicsTag::PHYSICS_WALL};
        m_ECS.AddComponent(leftWall, leftShape);

        Engine::Entity buttomWall = m_ECS.CreateEntity();
        m_ECS.AddComponent(buttomWall, Breakout::Transform({viewSize.x / 2.0f, viewSize.y}));
        m_ECS.AddComponent(buttomWall, Breakout::RigidBody({0, 0}));
        m_ECS.AddComponent(buttomWall, Breakout::CollisionEvents{{}, 0});

        Breakout::ColliderComponent buttomShape = {.rect = {viewSize.x, 0.1f},
                                                   .type = Breakout::ShapeType::SHAPE_RECTANGLE,
                                                   .tag = Breakout::PhysicsTag::PHYSICS_WALL};

        m_ECS.AddComponent(buttomWall, buttomShape);

        Engine::Entity rightWall = m_ECS.CreateEntity();
        m_ECS.AddComponent(rightWall, Breakout::Transform({viewSize.x, viewSize.y / 2.0f}));
        m_ECS.AddComponent(rightWall, Breakout::RigidBody({0, 0}));
        m_ECS.AddComponent(rightWall, Breakout::CollisionEvents{{}, 0});

        Breakout::ColliderComponent rightShape = {.rect = {0.1f, viewSize.y},
                                                  .type = Breakout::ShapeType::SHAPE_RECTANGLE,
                                                  .tag = Breakout::PhysicsTag::PHYSICS_WALL};
        m_ECS.AddComponent(rightWall, rightShape);
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
