#pragma once

#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <queue>

namespace Engine {

using Entity = uint32_t;
using ComponentType = uint8_t;

template <uint8_t ComponentCount, uint32_t EntityCount>
class EntityManager {
        using Signature = std::bitset<ComponentCount>;

       public:
        EntityManager();  // For a standard allocation of EntityCount Entities.

        Entity CreateEntity();
        void DestroyEntity(Entity entity);
        void SetSignature(Entity entity, Signature signature);
        Signature GetSignature(Entity entity);

       private:
        std::queue<Entity> m_AvailableEntities{};
        uint32_t m_LivingCount{};
        std::array<Signature, EntityCount> m_Signatures{};
};

//
//  Creates a new EntityManager with the templated ComponentCount, and a default max entity count of EntityCount.
//
template <uint8_t ComponentCount, uint32_t EntityCount>
EntityManager<ComponentCount, EntityCount>::EntityManager() {
        for (Entity entity = 0; entity < EntityCount; entity++) m_AvailableEntities.push(entity);
        m_Signatures.resize(EntityCount);
}

///
/// Retrives an available entity ID from the m_AvailableEntities.
/// Asserts that there are available entities.
///
template <uint8_t ComponentCount, uint32_t EntityCount>
Entity EntityManager<ComponentCount, EntityCount>::CreateEntity() {
        assert(m_LivingCount < EntityCount && "To many entities in existance");

        Entity entity = m_AvailableEntities.front();
        m_AvailableEntities.pop();
        m_LivingCount++;

        return entity;
}

///
/// Marks the entity as available, and resets its signature.
/// Asserts that the entity has a valid ID.
///
template <uint8_t ComponentCount, uint32_t EntityCount>
void EntityManager<ComponentCount, EntityCount>::DestroyEntity(Entity entity) {
        assert(entity < EntityCount && "Entity out of range");

        m_Signatures[entity].reset();

        m_AvailableEntities.push(entity);
        m_LivingCount--;
}

///
/// Assigns the specified signature to the provided entity.
/// Asserts that the ID of the entity is valid.
///
template <uint8_t ComponentCount, uint32_t EntityCount>
void EntityManager<ComponentCount, EntityCount>::SetSignature(Entity entity, Signature signature) {
        assert(entity < EntityCount && "Entity out of range");

        m_Signatures[entity] = signature;
}

///
/// Retrives the signature from the provided entity.
/// Asserts that the ID of the is valid.
///
template <uint8_t ComponentCount, uint32_t EntityCount>
std::bitset<ComponentCount> EntityManager<ComponentCount, EntityCount>::GetSignature(Entity entity) {
        assert(entity << EntityCount && "Entity out of range");

        return m_Signatures[entity];
}
}  // namespace Engine

// Based on [https://austinmorlan.com/posts/entity_component_system/]
