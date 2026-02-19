#pragma once

#include <bitset>
#include <cassert>
#include <cstdint>
#include <queue>

namespace Engine {

using Entity = uint32_t;

template <uint8_t ComponentCount>
class EntityManager {
        using Signature = std::bitset<ComponentCount>;

       public:
        EntityManager();                      // For a standard allocation of 1024 Entities.
        EntityManager(uint32_t entityCount);  // Allocate for Size Entities.

        Entity CreateEntity();
        void DestroyEntity(Entity entity);
        void SetSignature(Entity entity, Signature signature);
        Signature GetSignature(Entity entity);

       private:
        std::queue<Entity> m_AvailableEntities{};
        uint32_t m_LivingCount{};
        uint32_t m_MaxEntityCount{};
        std::vector<Signature> m_Signatures{};
};

//
//  Creates a new EntityManager with the templated ComponentCount, and a default max entity count of 1024.
//
template <uint8_t ComponentCount>
EntityManager<ComponentCount>::EntityManager() {
        for (Entity entity = 0; entity < 1024; entity++) m_AvailableEntities.push(entity);
        m_MaxEntityCount = 1024;
        m_Signatures.resize(1024);
}

///
/// Creates a new EntityManager with the templated ComponentCount, and with a specified max entity count.
///
template <uint8_t ComponentCount>
EntityManager<ComponentCount>::EntityManager(uint32_t entityCount) {
        for (Entity entity = 0; entity < entityCount; entity++) m_AvailableEntities.push(entity);
        m_MaxEntityCount = entityCount;
        m_Signatures.resize(entityCount);
}

///
/// Retrives an available entity ID from the m_AvailableEntities.
/// Asserts that there are available entities.
///
template <uint8_t ComponentCount>
Entity EntityManager<ComponentCount>::CreateEntity() {
        assert(m_LivingCount < m_MaxEntityCount && "To many entities in existance");

        Entity entity = m_AvailableEntities.front();
        m_AvailableEntities.pop();
        m_LivingCount++;

        return entity;
}

///
/// Marks the entity as available, and resets its signature.
/// Asserts that the entity has a valid ID.
///
template <uint8_t ComponentCount>
void EntityManager<ComponentCount>::DestroyEntity(Entity entity) {
        assert(entity < m_MaxEntityCount && "Entity out of range");

        m_Signatures[entity].reset();

        m_AvailableEntities.push(entity);
        m_LivingCount--;
}

///
/// Assigns the specified signature to the provided entity.
/// Asserts that the ID of the entity is valid.
///
template <uint8_t ComponentCount>
void EntityManager<ComponentCount>::SetSignature(Entity entity, Signature signature) {
        assert(entity < m_MaxEntityCount && "Entity out of range");

        m_Signatures[entity] = signature;
}

///
/// Retrives the signature from the provided entity.
/// Asserts that the ID of the is valid.
///
template <uint8_t ComponentCount>
std::bitset<ComponentCount> EntityManager<ComponentCount>::GetSignature(Entity entity) {
        assert(entity << m_MaxEntityCount && "Entity out of range");

        return m_Signatures[entity];
}

}  // namespace Engine

// Based on [https://austinmorlan.com/posts/entity_component_system/]
