#include "Engine/ResourceManager.h"

#include "SFML/Graphics/Texture.hpp"

template <>
sf::Texture* Engine::ResourceManager::Load<sf::Texture>(const std::string& path) {
        // Check if we have the Texture in the cache.
        auto it = m_TextureCache.find(path);
        if (it != m_TextureCache.end()) {
                // Mabey log it?
                m_TextureCount[path]++;
                return it->second;
        }

        // If it's not cached we load the Texture from source.
        sf::Texture* newTexture = new sf::Texture();
        if (!newTexture->loadFromFile(path)) {
                delete newTexture;  // Cleanup
                // Should log the missing Texture.
                return nullptr;
        }

        // We update the cache.
        m_TextureCache[path] = newTexture;
        m_TextureCount[path] = 1;
        return newTexture;
}
