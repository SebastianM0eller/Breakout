#pragma once
#include <memory>
#include <set>
#include <typeindex>
#include <unordered_map>

#include "Engine/ECS/Entity.h"

namespace Engine {

class System {
       public:
        std::set<Entity> m_Entities;
};

template <uint8_t ComponentCount>
class SystemManager {
        using Signature = std::bitset<ComponentCount>;

        template <typename T>
        std::shared_ptr<T> RegisterSystem();

        template <typename T>
        void SetSignature(Signature signature);

        void EntityDestroyed(Entity entity);
        void EntitySignatureChanged(Entity entity, Signature signature);

       private:
        std::unordered_map<std::type_index, Signature> m_Signatures{};
        std::unordered_map<std::type_index, std::shared_ptr<System>> m_Systems{};
};

template <uint8_t ComponentCount>
template <typename T>
std::shared_ptr<T> SystemManager<ComponentCount>::RegisterSystem() {
        std::type_index typeName(typeid(T));

        assert(m_Systems.find(typeName) == m_Systems.end() && "Registering a system more than once");

        std::shared_ptr<T> system = std::make_shared<T>();
        m_Systems.insert({typeName, system});
        return system;
}

template <uint8_t ComponentCount>
template <typename T>
void SystemManager<ComponentCount>::SetSignature(Signature signature) {
        std::type_index typeName(typeid(T));

        assert(m_Systems.find(typeName) != m_Systems.end() && "System used before Registering");

        m_Signatures.insert(typeName, signature);
}

template <uint8_t ComponentCount>
void SystemManager<ComponentCount>::EntityDestroyed(Entity entity) {
        for (auto const& [type, system] : m_Systems) {
                system->m_Entities.erase(entity);
        }
}

template <uint8_t ComponentCount>
void SystemManager<ComponentCount>::EntitySignatureChanged(Entity entity, Signature signature) {
        for (const auto& [type, system] : m_Systems) {
                auto const& systemSignature = m_Signatures[type];

                if ((signature & systemSignature) == systemSignature) {
                        system->m_Entities.insert(entity);
                } else {
                        system->m_Entities.erase(entity);
                }
        }
}

}  // namespace Engine
