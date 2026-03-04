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
}

void BreakoutGameLayer::RegisterSystems() {
        BreakoutPhysicsSystem::RegisterSelf(m_ECS);
        BreakoutUpdateEntityLocationSystem::RegisterSelf(m_ECS);
        BreakoutRenderSpritesSystem::RegisterSelf(m_ECS);
        BreakoutCollisionSystem::RegisterSelf(m_ECS);
}

void BreakoutGameLayer::RegisterEntities() {
        sf::Vector2f viewSize = Engine::Renderer::Get().GetViewSize();

        // Add the ball
        Engine::Entity entity = m_ECS.CreateEntity();
        m_ECS.AddComponent(entity, Transform({viewSize.x / 2.0f, viewSize.y - 60}));
        m_ECS.AddComponent(entity, Sprite({Engine::ManagedSprite("assets/Textures/BreakoutBallv2.png")}));
        m_ECS.AddComponent(entity, RigidBody({59, -150}));

        CollisionShape circleShape = {.type = ShapeType::CircleCollider, .shapeData = {.circle = {8.0f}}};
        m_ECS.AddComponent(entity, circleShape);

        // Add the paddle
        Engine::Entity paddleEntity = m_ECS.CreateEntity();
        m_ECS.AddComponent(paddleEntity, Transform({viewSize.x / 2.0f, viewSize.y - 45}));
        m_ECS.AddComponent(paddleEntity, Sprite({Engine::ManagedSprite("assets/Textures/BreakoutPaddle.png")}));
        m_ECS.AddComponent(paddleEntity, RigidBody({0, 0}));

        CollisionShape paddleShape = {.type = ShapeType::PaddleCollider, .shapeData = {.box = {56.0f, 6.0f}}};
        m_ECS.AddComponent(paddleEntity, paddleShape);

        // Add the walls.
        Engine::Entity topWall = m_ECS.CreateEntity();
        m_ECS.AddComponent(topWall, Transform({viewSize.x / 2.0f, 0}));
        m_ECS.AddComponent(topWall, RigidBody({0, 0}));

        CollisionShape topShape = {.type = ShapeType::BoxCollider, .shapeData = {.box = {viewSize.x, 0}}};
        m_ECS.AddComponent(topWall, topShape);

        Engine::Entity leftWall = m_ECS.CreateEntity();
        m_ECS.AddComponent(leftWall, Transform({0, viewSize.y / 2.0f}));
        m_ECS.AddComponent(leftWall, RigidBody({0, 0}));

        CollisionShape leftShape = {.type = ShapeType::BoxCollider, .shapeData = {.box = {0, viewSize.y}}};
        m_ECS.AddComponent(leftWall, leftShape);

        Engine::Entity buttomWall = m_ECS.CreateEntity();
        m_ECS.AddComponent(buttomWall, Transform({viewSize.x / 2.0f, viewSize.y}));
        m_ECS.AddComponent(buttomWall, RigidBody({0, 0}));

        CollisionShape buttomShape = {.type = ShapeType::BoxCollider, .shapeData = {.box = {viewSize.x, 0}}};
        m_ECS.AddComponent(buttomWall, buttomShape);

        Engine::Entity rightWall = m_ECS.CreateEntity();
        m_ECS.AddComponent(rightWall, Transform({viewSize.x, viewSize.y / 2.0f}));
        m_ECS.AddComponent(rightWall, RigidBody({0, 0}));

        CollisionShape rightShape = {.type = ShapeType::BoxCollider, .shapeData = {.box = {0, viewSize.y}}};
        m_ECS.AddComponent(rightWall, rightShape);
}

void BreakoutGameLayer::OnUpdate(float deltaTime) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) deltaTime *= 0.1f;
        m_ECS.GetSystem<BreakoutPhysicsSystem>()->OnUpdate(deltaTime, m_ECS);
        m_ECS.GetSystem<BreakoutCollisionSystem>()->OnUpdate(m_ECS);
        m_ECS.GetSystem<BreakoutUpdateEntityLocationSystem>()->OnUpdate(m_ECS);
}

void BreakoutGameLayer::OnRender() { m_ECS.GetSystem<BreakoutRenderSpritesSystem>()->OnRender(m_ECS); }
