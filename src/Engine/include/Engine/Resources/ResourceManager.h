#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"

namespace Engine {
class ResourceManager {
   public:
    ResourceManager(const ResourceManager&) = delete;

    ///
    /// Retrives the ResourceManager instance.
    /// If the ResourceManager is not yet initialized, it's constructed
    /// automatically.
    ///
    static ResourceManager& Get() {
        static ResourceManager instance;
        return instance;
    }

    ///
    /// Retrives a ptr to the resource. If the resource fails to load, a nullptr
    /// it returned. The ResourceManager has the ownership for the pointer.
    /// Calling delete outside of the ResourceManager will result in undefined
    /// behaviour. When the resource goes out of scope, Remove(path) should be
    /// called for proper cleanup.
    ///
    template <typename T>
    T* Load(const std::string& path);

    ///
    /// Decrements the internal count for the resource. If the count goes to 0,
    /// the resource is deleted. This method should only be called when a
    /// resources retrived from the ResourceManager goes out of scope. Calling
    /// this for the wrong reason will result in undefined behaviour.
    ///
    template <typename T>
    void Remove(const std::string& path);

    ///
    /// Decrements the internal count for the resource. If the count goes to 0,
    /// the resource is deleted. This method should only be called when a
    /// resources retrived from the ResourceManager goes out of scope. Calling
    /// this for the wrong reason will result in undefined behaviour.
    ///
    template <typename T>
    void Remove(T*);

    ///
    /// Returns a const reference to the string mapped to the texture.
    /// Can be used to automatically call the correct Remove, based on a ptr.
    /// Calling this without the ptr being mapped, will result in undefined
    /// behaviour.
    ///
    template <typename T>
    const std::string& GetString(T*);

   private:
    ResourceManager() {
    }

    std::unordered_map<std::string, sf::Texture*> m_TextureCache;
    std::unordered_map<sf::Texture*, std::string> m_TextureToString;
    std::unordered_map<std::string, int16_t> m_TextureCount;

    std::unordered_map<std::string, sf::Font*> m_FontCache;
    std::unordered_map<sf::Font*, std::string> m_FontToString;
    std::unordered_map<std::string, int16_t> m_FontCount;
};

// Texture
template <>
sf::Texture* ResourceManager::Load<sf::Texture>(const std::string& path);

template <>
void ResourceManager::Remove<sf::Texture>(const std::string& path);

template <>
void ResourceManager::Remove(sf::Texture* texture_ptr);

template <>
const std::string& ResourceManager::GetString(sf::Texture* texture);

// Font
template <>
sf::Font* ResourceManager::Load<sf::Font>(const std::string& path);

template <>
void ResourceManager::Remove<sf::Font>(const std::string& path);

template <>
void ResourceManager::Remove(sf::Font* font);

template <>
const std::string& ResourceManager::GetString(sf::Font* font);

}  // namespace Engine
