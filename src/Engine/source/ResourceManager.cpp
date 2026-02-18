#include "Engine/ResourceManager.h"

#include <iostream>

#include "SFML/Graphics/Texture.hpp"

template <>
sf::Texture* Engine::ResourceManager::Load<sf::Texture>(const std::string& path) {
        assert(path != "");
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

template <>
void Engine::ResourceManager::Remove<sf::Texture>(const std::string& path) {
        assert(path != "");

        // We check if the resource actually exists.
        auto it = m_TextureCount.find(path);
        if (it == m_TextureCount.end()) {
                std::cerr << "Tried to Remove an uninitialized Texture: " << path << "\n";
                return;
        }

        // We decrement the count, and do cleanup if its 0.
        m_TextureCount[path]--;
        if (m_TextureCount[path] <= 0) {
                delete m_TextureCache[path];  // Cleanup
                m_TextureCount.erase(path);
                m_TextureCache.erase(path);
        }
}
