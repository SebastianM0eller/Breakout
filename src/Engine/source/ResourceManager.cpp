#include "Engine/Resources/ResourceManager.h"

#include <cassert>
#include <iostream>

// Texture

template <>
sf::Texture* Engine::ResourceManager::Load<sf::Texture>(
    const std::string& path) {
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
        std::cerr << "Tried to Remove an uninitialized Texture: " << path
                  << "\n";
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

    std::string path = GetString(texture_ptr);
    Remove<sf::Texture>(path);
}

template <>
const std::string& Engine::ResourceManager::GetString(
    sf::Texture* texture_ptr) {
    assert(m_TextureToString.find(texture_ptr) != m_TextureToString.end() &&
           "There is no string associated with that ptr");

    return m_TextureToString[texture_ptr];
}

// Font

template <>
sf::Font* Engine::ResourceManager::Load<sf::Font>(const std::string& path) {
    assert(path != "");
    // Check if we have the Font in the cache.
    auto it = m_FontCache.find(path);
    if (it != m_FontCache.end()) {
        // Mabey log it?
        m_FontCount[path]++;
        return it->second;
    }

    // If it's not cached we load the Font from source.
    sf::Font* newFont = new sf::Font();
    if (!newFont->openFromFile(path)) {
        delete newFont;  // Cleanup
        // Should log the missing Font.
        return nullptr;
    }

    // We update the cache.
    m_FontCache[path] = newFont;
    m_FontCount[path] = 1;
    m_FontToString[newFont] = path;
    return newFont;
}

template <>
void Engine::ResourceManager::Remove<sf::Font>(const std::string& path) {
    assert(path != "");

    // We check if the resource actually exists.
    auto it = m_FontCount.find(path);
    if (it == m_FontCount.end()) {
        std::cerr << "Tried to Remove an uninitialized Font: " << path << "\n";
        return;
    }

    // We decrement the count, and do cleanup if its 0.
    m_FontCount[path]--;
    if (m_FontCount[path] <= 0) {
        m_FontToString.erase(m_FontCache[path]);  // Cleanup
        delete m_FontCache[path];
        m_FontCount.erase(path);
        m_FontCache.erase(path);
    }
}

template <>
void Engine::ResourceManager::Remove(sf::Font* font) {
    assert(font && "Cannot remove a nullptr");

    std::string path = GetString(font);
    Remove<sf::Font>(path);
}

template <>
const std::string& Engine::ResourceManager::GetString(sf::Font* font) {
    assert(m_FontToString.find(font) != m_FontToString.end() &&
           "There is no string associated with that ptr");

    return m_FontToString[font];
}
