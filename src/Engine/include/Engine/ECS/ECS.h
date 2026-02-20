#pragma once

#include <cstdint>
#include <memory>

#include "Engine/ECS/ComponentManager.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/System.h"
namespace Engine {
template <uint8_t ComponentCount, uint32_t EntityCount>
class ECS {
       public:
        using Signature = std::bitset<ComponentCount>;
        void Init() {
                m_ComponentManager = std::make_unique<ComponentManager<EntityCount>>();
                m_EntityManager = std::make_unique<EntityManager<ComponentCount, EntityCount>>();
                m_SystemManager = std::make_unique<SystemManager<ComponentCount>>();
        }

        Entity CreateEntity() { return m_EntityManager->CreateEntity(); }

        void DestroyEntity(Entity entity) {
                m_EntityManager->DestroyEntity(entity);
                m_ComponentManager->EntityDestroyed(entity);
                m_SystemManager->EntityDestroyed(entity);
        }

        template <typename T>
        void RegisterComponent() {
                m_ComponentManager->template RegisterComponent<T>();
        }

        template <typename T>
        void AddComponent(Entity entity, T componenet) {
                m_ComponentManager->template AddComponent<T>(entity, componenet);

                Signature signature = m_EntityManager->GetSignature(entity);
                signature.set(m_ComponentManager->template GetComponentType<T>(), true);
                m_EntityManager->SetSignature(entity, signature);

                m_SystemManager->EntitySignatureChanged(entity, signature);
        }

        template <typename T>
        void RemoveComponent(Entity entity) {
                m_ComponentManager->template RemoveComponent<T>(entity);

                Signature signature = m_EntityManager->GetSignature(entity);
                signature.set(m_ComponentManager->template GetComponentType<T>(), false);
                m_EntityManager->SetSignature(entity, signature);

                m_SystemManager->EntitySignatureChanged(entity, signature);
        }

        template <typename T>
        T& GetComponent(Entity entity) {
                return m_ComponentManager->template GetComponent<T>(entity);
        }

        template <typename T>
        ComponentType GetComponentType() {
                return m_ComponentManager->template GetComponentType<T>();
        }

        template <typename T>
        std::shared_ptr<T> RegisterSystem() {
                return m_SystemManager->template RegisterSystem<T>();
        }

        template <typename T>
        void SetSystemSignature(Signature signature) {
                m_SystemManager->template SetSystemSignature<T>(signature);
        }

       private:
        std::unique_ptr<ComponentManager<EntityCount>> m_ComponentManager;
        std::unique_ptr<EntityManager<ComponentCount, EntityCount>> m_EntityManager;
        std::unique_ptr<SystemManager<ComponentCount>> m_SystemManager;
};
}  // namespace Engine
