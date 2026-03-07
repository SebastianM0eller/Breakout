#include "BreakoutGameLayer.h"

#include "Breakout/BreakoutComponents.h"
#include "Breakout/BreakoutSystems.h"
#include "Components.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Renderer.h"
#include "Engine/Resources/Texture.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "Systems/CollisionDetectionSystem.h"
#include "Systems/CollisionResolutionSystem.h"

BreakoutGameLayer::BreakoutGameLayer() {
        m_ECS.Init();
        RegisterComponents();
        RegisterSystems();
        RegisterEntities();
}

void BreakoutGameLayer::RegisterComponents() {
        m_ECS.RegisterComponent<RigidBody>();
        m_ECS.RegisterComponent<Transform>();
        m_ECS.RegisterComponent<Sprite>();
        m_ECS.RegisterComponent<Player>();
        m_ECS.RegisterComponent<CollisionShape>();
        m_ECS.RegisterComponent<Breakout::Transform>();
        m_ECS.RegisterComponent<Breakout::ColliderComponent>();
        m_ECS.RegisterComponent<Breakout::CollisionEvents>();
}

void BreakoutGameLayer::RegisterSystems() {
        BreakoutPhysicsSystem::RegisterSelf(m_ECS);
        BreakoutUpdateEntityLocationSystem::RegisterSelf(m_ECS);
        BreakoutRenderSpritesSystem::RegisterSelf(m_ECS);
        PlayerMovementSystem::RegisterSelf(m_ECS);
        Breakout::CollisionDetectionSystem::RegisterSelf(m_ECS);
        Breakout::CollisionResolutionSystem::RegisterSelf(m_ECS);
}

void BreakoutGameLayer::RegisterEntities() {
        sf::Vector2f viewSize = Engine::Renderer::Get().GetViewSize();

        // Add the ball
        Engine::Entity entity = m_ECS.CreateEntity();
        m_ECS.AddComponent(entity, Breakout::Transform({viewSize.x / 2.0f, viewSize.y - 60}));
        m_ECS.AddComponent(entity, Sprite({Engine::ManagedSprite("assets/Textures/BreakoutBallv2.png")}));
        m_ECS.AddComponent(entity, RigidBody({0, -200}));
        m_ECS.AddComponent(entity, Breakout::CollisionEvents{{}, 0});

        Breakout::ColliderComponent circleCollider = {
            .circle = {8.0f}, .type = Breakout::ShapeType::SHAPE_CIRCLE, .tag = Breakout::PhysicsTag::PHYSICS_BALL};
        m_ECS.AddComponent(entity, circleCollider);

        // Add the paddle
        Engine::Entity paddleEntity = m_ECS.CreateEntity();
        m_ECS.AddComponent(paddleEntity, Breakout::Transform({viewSize.x / 2.0f, viewSize.y - 45}));
        m_ECS.AddComponent(paddleEntity, Sprite({Engine::ManagedSprite("assets/Textures/BreakoutPaddle.png")}));
        m_ECS.AddComponent(paddleEntity, RigidBody({0, 0}));
        m_ECS.AddComponent(paddleEntity, Player{});
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
        m_ECS.AddComponent(topWall, RigidBody({0, 0}));
        m_ECS.AddComponent(topWall, Breakout::CollisionEvents{{}, 0});

        Breakout::ColliderComponent topShape = {.rect = {viewSize.x, 0},
                                                .type = Breakout::ShapeType::SHAPE_RECTANGLE,
                                                .tag = Breakout::PhysicsTag::PHYSICS_WALL};
        m_ECS.AddComponent(topWall, topShape);

        Engine::Entity leftWall = m_ECS.CreateEntity();
        m_ECS.AddComponent(leftWall, Breakout::Transform({0.1f, viewSize.y / 2.0f}));
        m_ECS.AddComponent(leftWall, RigidBody({0, 0}));
        m_ECS.AddComponent(leftWall, Breakout::CollisionEvents{{}, 0});

        Breakout::ColliderComponent leftShape = {.rect = {0, viewSize.y},
                                                 .type = Breakout::ShapeType::SHAPE_RECTANGLE,
                                                 .tag = Breakout::PhysicsTag::PHYSICS_WALL};
        m_ECS.AddComponent(leftWall, leftShape);

        Engine::Entity buttomWall = m_ECS.CreateEntity();
        m_ECS.AddComponent(buttomWall, Breakout::Transform({viewSize.x / 2.0f, viewSize.y}));
        m_ECS.AddComponent(buttomWall, RigidBody({0, 0}));
        m_ECS.AddComponent(buttomWall, Breakout::CollisionEvents{{}, 0});

        Breakout::ColliderComponent buttomShape = {.rect = {viewSize.x, 0.1f},
                                                   .type = Breakout::ShapeType::SHAPE_RECTANGLE,
                                                   .tag = Breakout::PhysicsTag::PHYSICS_WALL};

        m_ECS.AddComponent(buttomWall, buttomShape);

        Engine::Entity rightWall = m_ECS.CreateEntity();
        m_ECS.AddComponent(rightWall, Breakout::Transform({viewSize.x, viewSize.y / 2.0f}));
        m_ECS.AddComponent(rightWall, RigidBody({0, 0}));
        m_ECS.AddComponent(rightWall, Breakout::CollisionEvents{{}, 0});

        Breakout::ColliderComponent rightShape = {.rect = {0.1f, viewSize.y},
                                                  .type = Breakout::ShapeType::SHAPE_RECTANGLE,
                                                  .tag = Breakout::PhysicsTag::PHYSICS_WALL};
        m_ECS.AddComponent(rightWall, rightShape);
}

void BreakoutGameLayer::OnUpdate(float deltaTime) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) deltaTime *= 0.1f;
        m_ECS.GetSystem<PlayerMovementSystem>()->OnUpdate(m_ECS);
        m_ECS.GetSystem<BreakoutPhysicsSystem>()->OnUpdate(deltaTime, m_ECS);
        m_ECS.GetSystem<Breakout::CollisionDetectionSystem>()->OnUpdate(m_ECS);
        m_ECS.GetSystem<Breakout::CollisionResolutionSystem>()->OnUpdate(m_ECS);
        m_ECS.GetSystem<BreakoutUpdateEntityLocationSystem>()->OnUpdate(m_ECS);
}

void BreakoutGameLayer::OnRender() { m_ECS.GetSystem<BreakoutRenderSpritesSystem>()->OnRender(m_ECS); }
