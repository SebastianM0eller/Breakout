#pragma once
#include <array>
#include <cassert>
#include <cstdint>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "Engine/ECS/Entity.h"

namespace Engine {

class IComponentArray {
       public:
        virtual ~IComponentArray() = default;
        virtual void EntityDestroyed(Entity entity) = 0;
};

template <typename T, uint32_t EntityCount>
class ComponentArray : public IComponentArray {
       public:
        void InsertData(Entity entity, T component);
        void RemoveData(Entity entity);
        T& GetData(Entity entity);
        void EntityDestroyed(Entity entity) override;

       private:
        std::array<T, EntityCount> m_ComponentArray;
        std::unordered_map<Entity, uint32_t> m_EntityToIndexMap;
        std::unordered_map<uint32_t, Entity> m_IndexToEntityMap;
        uint32_t m_Size{0};
};

///
/// Inserts the component, and maps it to the entity.
/// Asserts that the entity doesn't already have the component.
///
template <typename T, uint32_t EntityCount>
void ComponentArray<T, EntityCount>::InsertData(Entity entity, T component) {
        assert(m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end() &&
               "Component added to the same entity more than once");

        // Add the component to the end of the continous memory
        // Update the maps and size.
        uint32_t newIndex = m_Size;
        m_EntityToIndexMap[entity] = newIndex;
        m_IndexToEntityMap[newIndex] = entity;
        m_ComponentArray[newIndex] = component;
        m_Size++;
}

template <typename T, uint32_t EntityCount>
void ComponentArray<T, EntityCount>::RemoveData(Entity entity) {
        assert(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end() &&
               " Trying to remove a non-existing component");

        // Copy last element to deleted elements place to maintain density.
        uint32_t indexOfRemovedEntity = m_EntityToIndexMap[entity];
        uint32_t indexOfLastElement = m_Size - 1;
        m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];  // Does it need to be deleted?

        // Update the maps and size.
        Entity entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
        m_EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
        m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

        m_EntityToIndexMap.erase(entity);
        m_IndexToEntityMap.erase(indexOfLastElement);

        m_Size--;
}

template <typename T, uint32_t EntityCount>
T& ComponentArray<T, EntityCount>::GetData(Entity entity) {
        assert(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end() &&
               "Trying to retrive a non-existing component");

        return m_ComponentArray[m_EntityToIndexMap[entity]];
}

template <typename T, uint32_t EntityCount>
void ComponentArray<T, EntityCount>::EntityDestroyed(Entity entity) {
        if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end()) RemoveData(entity);
}

/*
template <uint32_t EntityCount>
class ComponentManager {
       public:
        template <typename T>
        void RegisterComponent();

        template <typename T>
        ComponentType GetComponentType();

        template <typename T>
        void AddComponent(Entity entity, T component);

        template <typename T>
        void RemoveComponent(Entity entity);

        template <typename T>
        T& GetComponent(Entity entity);

        void EntityDestroyed(Entity entity);

       private:
        std::unordered_map<std::type_index, ComponentType> m_ComponentTypes{};
        std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> m_ComponentArrays{};
        ComponentType m_NextComponentType{0};

        template <typename T>
        std::shared_ptr<ComponentArray<T, EntityCount>> GetComponentArray();
};

template <uint32_t EntityCount>
template <typename T>
void ComponentManager<EntityCount>::RegisterComponent() {
        std::type_index typeName(typeid(T));

        assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() &&
               "Registering same component more than once");

        m_ComponentTypes.insert({typeName, m_NextComponentType});
        m_ComponentArrays.insert({typeName, std::make_shared<ComponentArray<T, EntityCount>>()});

        m_NextComponentType++;
}

template <uint32_t EntityCount>
template <typename T>
ComponentType ComponentManager<EntityCount>::GetComponentType() {
        std::type_index typeName(typeid(T));

        assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() &&
               "Component was not registered before being used");

        return m_ComponentTypes[typeName];
}

template <uint32_t EntityCount>
template <typename T>
void ComponentManager<EntityCount>::AddComponent(Entity entity, T component) {
        GetComponentArray<T>()->InsertData(entity, component);
}

template <uint32_t EntityCount>
template <typename T>
void ComponentManager<EntityCount>::RemoveComponent(Entity entity) {
        GetComponentArray<T>()->RemoveData(entity);
}

template <uint32_t EntityCount>
template <typename T>
T& ComponentManager<EntityCount>::GetComponent(Entity entity) {
        return GetComponentArray<T>()->GetData(entity);
}

template <uint32_t EntityCount>
void ComponentManager<EntityCount>::EntityDestroyed(Entity entity) {
        for (auto const& [type, componentArray] : m_ComponentArrays) {
                componentArray->EntityDestroyed(entity);
        }
}

template <uint32_t EntityCount>
template <typename T>
std::shared_ptr<ComponentArray<T, EntityCount>> ComponentManager<EntityCount>::GetComponentArray() {
        std::type_index typeName(typeid(T));

        assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered before use.");

        return std::static_pointer_cast<ComponentArray<T, EntityCount>>(m_ComponentArrays[typeName]);
}

*/

}  // namespace Engine

// Based on [https://austinmorlan.com/posts/entity_component_system/]
