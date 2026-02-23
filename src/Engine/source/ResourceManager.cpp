#include "Engine/Resources/ResourceManager.h"

#include <cassert>
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
        m_TextureToString[newTexture] = path;
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
                m_TextureToString.erase(m_TextureCache[path]);  // Cleanup
                delete m_TextureCache[path];
                m_TextureCount.erase(path);
                m_TextureCache.erase(path);
        }
}

template <>
void Engine::ResourceManager::Remove(sf::Texture* texture_ptr) {
        assert(texture_ptr && "Cannot remove a nullptr");

        Remove<sf::Texture>(GetString(texture_ptr));
}

template <>
const std::string& Engine::ResourceManager::GetString(sf::Texture* texture_ptr) {
        assert(m_TextureToString.find(texture_ptr) != m_TextureToString.end() &&
               "There is no string associated with that ptr");

        return m_TextureToString[texture_ptr];
}
