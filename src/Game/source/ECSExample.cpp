#include "Example/ECSExample.h"

#include <bitset>

#include "Engine/ECS/Entity.h"
#include "Example/ExampleStructs.h"
#include "Example/ExampleSystems.h"

using Signature = std::bitset<32>;

ECSLayer::ECSLayer() {
        m_ECS.Init();
        RegisterComponents();
        RegisterSystems();
        RegisterEntities();
}

void ECSLayer::RegisterComponents() {
        m_ECS.RegisterComponent<Location>();
        m_ECS.RegisterComponent<RigidBody>();
        m_ECS.RegisterComponent<LogLocation>();
}

void ECSLayer::RegisterSystems() {
        m_ECS.RegisterSystem<PhysicsSystem>();

        Signature physicsSignature;
        physicsSignature.set(m_ECS.GetComponentType<Location>());
        physicsSignature.set(m_ECS.GetComponentType<RigidBody>());
        m_ECS.SetSystemSignature<PhysicsSystem>(physicsSignature);

        m_ECS.RegisterSystem<LocationLoggingSystem>();

        Signature loggingSignature;
        loggingSignature.set(m_ECS.GetComponentType<Location>());
        loggingSignature.set(m_ECS.GetComponentType<LogLocation>());
        m_ECS.SetSystemSignature<LocationLoggingSystem>(loggingSignature);
}

void ECSLayer::RegisterEntities() {
        Engine::Entity entity = m_ECS.CreateEntity();
        m_ECS.AddComponent(entity, Location({40.0f, 40.0f}));
        m_ECS.AddComponent(entity, RigidBody({0, 0}, {1, 0}));
        m_ECS.AddComponent(entity, LogLocation());
};

void ECSLayer::OnUpdate(float deltaTime) {
        m_ECS.GetSystem<PhysicsSystem>()->OnUpdate(deltaTime, m_ECS);
        m_ECS.GetSystem<LocationLoggingSystem>()->OnUpdate(m_ECS);
}
