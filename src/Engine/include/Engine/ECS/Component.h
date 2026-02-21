#pragma once
#include <array>
#include <cassert>
#include <cstdint>
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

///
/// Removes the component of the entity, from the ComponentArray while keeping the ComponentArray dense.
/// Asserts that the entity has the component trying to be removed.
///
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

///
/// Retrieves a reference to the component associated with the entity.
/// Asserts that the entity has the component trying to be retrived.
///
template <typename T, uint32_t EntityCount>
T& ComponentArray<T, EntityCount>::GetData(Entity entity) {
        assert(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end() &&
               "Trying to retrive a non-existing component");

        return m_ComponentArray[m_EntityToIndexMap[entity]];
}

///
/// Removes the component associated with the destroyed entity, while keeping the ComponentArray dense.
/// If the entity has no associated component, this does nothing.
///
template <typename T, uint32_t EntityCount>
void ComponentArray<T, EntityCount>::EntityDestroyed(Entity entity) {
        if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end()) RemoveData(entity);
}

}  // namespace Engine

// Based on [https://austinmorlan.com/posts/entity_component_system/]
