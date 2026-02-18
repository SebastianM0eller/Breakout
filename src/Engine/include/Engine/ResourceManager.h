#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "SFML/Graphics/Texture.hpp"

namespace Engine {
class ResourceManager {
       public:
        ResourceManager(const ResourceManager&) = delete;

        ///
        /// Retrives the ResourceManager instance.
        /// If the ResourceManager is not yet initialized, it's constructed automatically.
        ///
        static ResourceManager& Get() {
                static ResourceManager instance;
                return instance;
        }

        template <typename T>
        T* Load(const std::string& path);

       private:
        ResourceManager() {}

        std::unordered_map<std::string, sf::Texture*> m_TextureCache;
        std::unordered_map<std::string, int16_t> m_TextureCount;
};

template <>
sf::Texture* Engine::ResourceManager::Load<sf::Texture>(const std::string& path);

}  // namespace Engine
