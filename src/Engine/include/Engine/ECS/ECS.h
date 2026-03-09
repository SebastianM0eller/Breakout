#pragma once

#include <cstdint>
#include <memory>

#include "Engine/ECS/ComponentManager.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EventManager.h"
#include "Engine/ECS/System.h"
namespace Engine {
template <uint8_t ComponentCount, uint32_t EntityCount>
class ECS {
       public:
        using Signature = std::bitset<ComponentCount>;

        ///
        /// Used to initialize the ECS.
        /// Internally initializes the different managers.
        ///
        void Init() {
                m_ComponentManager = std::make_unique<ComponentManager<EntityCount>>();
                m_EntityManager = std::make_unique<EntityManager<ComponentCount, EntityCount>>();
                m_SystemManager = std::make_unique<SystemManager<ComponentCount>>();
                m_EventManager = std::make_unique<EventManager>();
        }

        ///
        /// Used to retrieve the id for an available entity ID.
        /// An assert is triggered in debug, if there are no available entities.
        ///
        Entity CreateEntity() { return m_EntityManager->CreateEntity(); }

        ///
        /// Used to inform the different managers that an entity has been destroyed.
        ///
        void DestroyEntity(Entity entity) {
                m_EntityManager->DestroyEntity(entity);
                m_ComponentManager->EntityDestroyed(entity);
                m_SystemManager->EntityDestroyed(entity);
        }

        ///
        /// Used to register a new component.
        /// An assert is triggered if the component has already been registered.
        ///
        template <typename T>
        void RegisterComponent() {
                m_ComponentManager->template RegisterComponent<T>();
        }

        ///
        /// Used to add a component to an entity.
        /// An assert is triggered in debug, if the component has not yet been registered.
        /// An assert is also triggered if the entity already has an associated component.
        ///
        template <typename T>
        void AddComponent(Entity entity, T componenet) {
                m_ComponentManager->template AddComponent<T>(entity, componenet);

                Signature signature = m_EntityManager->GetSignature(entity);
                signature.set(m_ComponentManager->template GetComponentType<T>(), true);
                m_EntityManager->SetSignature(entity, signature);

                m_SystemManager->EntitySignatureChanged(entity, signature);
        }

        ///
        /// Used to remove a component from an entity.
        /// An assert is triggered in debug, if the entity does not have the component, or if the component is not
        /// registered.
        ///
        template <typename T>
        void RemoveComponent(Entity entity) {
                m_ComponentManager->template RemoveComponent<T>(entity);

                Signature signature = m_EntityManager->GetSignature(entity);
                signature.set(m_ComponentManager->template GetComponentType<T>(), false);
                m_EntityManager->SetSignature(entity, signature);

                m_SystemManager->EntitySignatureChanged(entity, signature);
        }

        ///
        /// Retrives a reference to the component for the specified entity.
        ///
        template <typename T>
        T& GetComponent(Entity entity) {
                return m_ComponentManager->template GetComponent<T>(entity);
        }

        ///
        /// Retrieves the ComponentType (ID) for the specified component T.
        /// An assert is triggered of the component is not registered.
        ///
        template <typename T>
        ComponentType GetComponentType() {
                return m_ComponentManager->template GetComponentType<T>();
        }

        ///
        /// Used to register a new system.
        /// A system should be registered before the enitities.
        /// An assert it triggered if the system has already been registered.
        ///
        template <typename T>
        std::shared_ptr<T> RegisterSystem() {
                return m_SystemManager->template RegisterSystem<T>();
        }

        ///
        /// Used to set the signature of a system.
        /// An assert is triggered if the system has not been registered.
        ///
        template <typename T>
        void SetSystemSignature(Signature signature) {
                m_SystemManager->template SetSignature<T>(signature);
        }

        ///
        /// Used to retrieve a shared_ptr to the system.
        /// An assert is triggered if the system has not been registered yet.
        ///
        template <typename T>
        std::shared_ptr<T> GetSystem() {
                return m_SystemManager->template GetSystem<T>();
        }

        ///
        /// Used to register a callback for the given EventType.
        ///
        template <typename EventType>
        void AddListner(std::function<void(const EventType&)> callback) {
                m_EventManager->AddListner<EventType>(callback);
        }

        ///
        /// Distributes the event to the provided listners.
        /// If there are no listers for the event, nothing happens.
        ///
        template <typename EventType>
        void SendEvent(const EventType& event) {
                m_EventManager->SendEvent(event);
        }

       private:
        std::unique_ptr<ComponentManager<EntityCount>> m_ComponentManager;
        std::unique_ptr<EntityManager<ComponentCount, EntityCount>> m_EntityManager;
        std::unique_ptr<SystemManager<ComponentCount>> m_SystemManager;
        std::unique_ptr<EventManager> m_EventManager;
};
}  // namespace Engine

// Based on [https://austinmorlan.com/posts/entity_component_system/]
