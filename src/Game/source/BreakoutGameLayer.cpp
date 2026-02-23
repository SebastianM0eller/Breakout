#include "BreakoutGameLayer.h"

#include "Breakout/BreakoutComponents.h"
#include "Breakout/BreakoutSystems.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Resources/Texture.h"

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
        // Add the ball
        Engine::Entity entity = m_ECS.CreateEntity();
        m_ECS.AddComponent(entity, Transform({400, 300}));
        m_ECS.AddComponent(entity, Sprite({Engine::ManagedSprite("assets/Textures/BreakoutBallv2.png")}));
        m_ECS.AddComponent(entity, RigidBody({20, 20}));

        CollisionShape circleShape = {.type = ShapeType::CircleCollider, .shapeData = {.circle = {8.0f}}};
        m_ECS.AddComponent(entity, circleShape);

        // Add the paddle
        Engine::Entity paddleEntity = m_ECS.CreateEntity();
        m_ECS.AddComponent(paddleEntity, Transform({400, 50}));
        m_ECS.AddComponent(paddleEntity, Sprite({Engine::ManagedSprite("assets/Textures/BreakoutPaddle.png")}));
}

void BreakoutGameLayer::OnUpdate(float deltaTime) {
        m_ECS.GetSystem<BreakoutPhysicsSystem>()->OnUpdate(deltaTime, m_ECS);
        m_ECS.GetSystem<BreakoutCollisionSystem>()->OnUpdate(m_ECS);
        m_ECS.GetSystem<BreakoutUpdateEntityLocationSystem>()->OnUpdate(m_ECS);
}

void BreakoutGameLayer::OnRender() { m_ECS.GetSystem<BreakoutRenderSpritesSystem>()->OnRender(m_ECS); }
