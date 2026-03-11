#pragma once

#include <cstdint>
#include <memory>
#include <type_traits>
#include <typeindex>

#include "Engine/ECS/Component.h"
#include "Engine/ECS/Entity.h"

namespace Engine {

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

        template <typename T>
        bool ContainsComponent(Entity entity);

        void EntityDestroyed(Entity entity);

       private:
        std::unordered_map<std::type_index, ComponentType> m_ComponentTypes{};
        std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> m_ComponentArrays{};
        ComponentType m_NextComponentType{0};

        template <typename T>
        std::shared_ptr<ComponentArray<T, EntityCount>> GetComponentArray();
};

///
/// Creates a new ComponentArray, to hold the components.
/// Asserts that the componenttype has not been registered before.
///
template <uint32_t EntityCount>
template <typename T>
void ComponentManager<EntityCount>::RegisterComponent() {
        std::type_index typeName(typeid(T));

        assert(m_ComponentTypes.find(typeName) == m_ComponentTypes.end() &&
               "Registering same component more than once");

        m_ComponentTypes.insert({typeName, m_NextComponentType});

        // Only add the component array if is contains data.
        // Saves EntityCount Bytes
        if constexpr (!std::is_empty_v<T>) {
                m_ComponentArrays.insert({typeName, std::make_shared<ComponentArray<T, EntityCount>>()});
        }

        m_NextComponentType++;
}

///
/// Retrives the ComponentType.
/// Asserts that the componenttype was registered before being accesed.
///
template <uint32_t EntityCount>
template <typename T>
ComponentType ComponentManager<EntityCount>::GetComponentType() {
        std::type_index typeName(typeid(T));

        assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() &&
               "Component was not registered before being acessed");

        return m_ComponentTypes[typeName];
}

///
/// Add the specified component to the provided entity.
/// Asserts that the entity does not already have a component of that type.
///
template <uint32_t EntityCount>
template <typename T>
void ComponentManager<EntityCount>::AddComponent(Entity entity, T component) {
        // If the struct is empty, it has no corrosponding array.
        if constexpr (!std::is_empty_v<T>) {
                GetComponentArray<T>()->InsertData(entity, component);
        }
}

///
/// Removes the componenttype from the specified entity.
/// Asserts that the entity does have a component of that type.
///
template <uint32_t EntityCount>
template <typename T>
void ComponentManager<EntityCount>::RemoveComponent(Entity entity) {
        // If the struct is empty, is has no corrosponding array.
        if constexpr (!std::is_empty_v<T>) {
                GetComponentArray<T>()->RemoveData(entity);
        }
}

///
/// Retrives a reference to the component from the specified entity.
/// Asserts that the entity has the specified component.
///
template <uint32_t EntityCount>
template <typename T>
T& ComponentManager<EntityCount>::GetComponent(Entity entity) {
        static_assert(!std::is_empty_v<T>, "Can't retrieve data for an empty struct");
        return GetComponentArray<T>()->GetData(entity);
}

///
/// Removes the entity's components from the respective ComponentArrays, while keeping them dense.
///
template <uint32_t EntityCount>
void ComponentManager<EntityCount>::EntityDestroyed(Entity entity) {
        for (auto const& [type, componentArray] : m_ComponentArrays) {
                componentArray->EntityDestroyed(entity);
        }
}

///
/// Returns wether the entity contains the specified component or not.
///
template <uint32_t EntityCount>
template <typename T>
bool ComponentManager<EntityCount>::ContainsComponent(Entity entity) {
        return GetComponentArray<T>()->HasData(entity);
}

///
/// Retrives a shared_ptr to the dense ComponentArray for the specified type.
/// Asserts that the component has been registered.
///
template <uint32_t EntityCount>
template <typename T>
std::shared_ptr<ComponentArray<T, EntityCount>> ComponentManager<EntityCount>::GetComponentArray() {
        std::type_index typeName(typeid(T));

        assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered before use.");

        return std::static_pointer_cast<ComponentArray<T, EntityCount>>(m_ComponentArrays[typeName]);
}

}  // namespace Engine

// Based on [https://austinmorlan.com/posts/entity_component_system/]
