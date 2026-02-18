#pragma once

#include <bitset>
#include <cstdint>
#include <queue>

namespace Engine {

struct Entity {
        uint32_t id;
};

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
        std::vector<Signature> m_Signatures{};
        uint32_t m_LivingCount{};
};

}  // namespace Engine

// Based on [https://austinmorlan.com/posts/entity_component_system/]
