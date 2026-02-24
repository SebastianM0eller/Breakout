#include "BreakoutGameLayer.h"

#include "Breakout/BreakoutComponents.h"
#include "Breakout/BreakoutSystems.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Renderer.h"
#include "Engine/Resources/Texture.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"

using Signature = std::bitset<32>;

BreakoutGameLayer::BreakoutGameLayer() {
        m_ECS.Init();
        RegisterComponents();
        RegisterSystems();
        RegisterEntities();
}

void BreakoutGameLayer::RegisterComponents() {
        m_ECS.RegisterComponent<RigidBody>();
        m_ECS.RegisterComponent<CollisionShape>();
        m_ECS.RegisterComponent<Transform>();
        m_ECS.RegisterComponent<Sprite>();
        m_ECS.RegisterComponent<HorizontalMovement>();
}

void BreakoutGameLayer::RegisterSystems() {
        BreakoutPhysicsSystem::RegisterSelf(m_ECS);
        BreakoutUpdateEntityLocationSystem::RegisterSelf(m_ECS);
        BreakoutRenderSpritesSystem::RegisterSelf(m_ECS);
        BreakoutCollisionSystem::RegisterSelf(m_ECS);
        BreakoutPaddleMovementSystem::RegisterSelf(m_ECS);
}

void BreakoutGameLayer::RegisterEntities() {
        sf::Vector2f viewSize = Engine::Renderer::Get().GetViewSize();
        // Add the ball
        Engine::Entity entity = m_ECS.CreateEntity();
        m_ECS.AddComponent(entity, Transform({viewSize.x / 2.0f, viewSize.y - 60}));
        m_ECS.AddComponent(entity, Sprite({Engine::ManagedSprite("assets/Textures/BreakoutBallv2.png")}));
        m_ECS.AddComponent(entity, RigidBody({0, -150}));

        CollisionShape circleShape = {.type = ShapeType::CircleCollider, .shapeData = {.circle = {8.0f}}};
        m_ECS.AddComponent(entity, circleShape);

        // Add the paddle
        Engine::Entity paddleEntity = m_ECS.CreateEntity();
        m_ECS.AddComponent(paddleEntity, Transform({viewSize.x / 2.0f, viewSize.y - 50}));
        m_ECS.AddComponent(paddleEntity, Sprite({Engine::ManagedSprite("assets/Textures/BreakoutPaddle.png")}));

        HorizontalMovement paddleMovement;
        paddleMovement.movementSpeed = 150;
        paddleMovement.moveRightKeys.emplace_back(sf::Keyboard::Key::D);
        paddleMovement.moveLeftKeys.emplace_back(sf::Keyboard::Key::A);
        m_ECS.AddComponent(paddleEntity, paddleMovement);
}

void BreakoutGameLayer::OnUpdate(float deltaTime) {
        m_ECS.GetSystem<BreakoutPhysicsSystem>()->OnUpdate(deltaTime, m_ECS);
        m_ECS.GetSystem<BreakoutPaddleMovementSystem>()->OnUpdate(deltaTime, m_ECS);
        m_ECS.GetSystem<BreakoutCollisionSystem>()->OnUpdate(m_ECS);
        m_ECS.GetSystem<BreakoutUpdateEntityLocationSystem>()->OnUpdate(m_ECS);
}

void BreakoutGameLayer::OnRender() { m_ECS.GetSystem<BreakoutRenderSpritesSystem>()->OnRender(m_ECS); }
