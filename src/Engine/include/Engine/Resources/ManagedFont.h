#pragma once

#include <string>

#include "Engine/Resources/ResourceManager.h"
#include "SFML/Graphics/Font.hpp"

namespace Engine {
class ManagedFont {
       public:
        ManagedFont() : m_Font(nullptr) {}  // We need default initialization, for it to work with the ECS.

        // Used to load the actual font.
        ManagedFont(const std::string& path) : m_Font(ResourceManager::Get().Load<sf::Font>(path)) {}

        // Override the default copy.
        ManagedFont(const ManagedFont& other) : m_Font(nullptr) {
                // We check if the other has a valid ptr.
                if (other.m_Font) {
                        ResourceManager& instance = ResourceManager::Get();
                        m_Font = instance.Load<sf::Font>(instance.GetString(other.m_Font));
                }
        }

        // Override the assignment operator.
        ManagedFont& operator=(const ManagedFont& other) {
                if (this == &other) return *this;

                ResourceManager& instance = ResourceManager::Get();

                // Decrese the count for the original font.
                if (m_Font) {
                        instance.Remove(m_Font);
                        m_Font = nullptr;  // In case the other doesn't have a font.
                }

                // If the other has a font, we update our font.
                if (other.m_Font) {
                        m_Font = instance.Load<sf::Font>(instance.GetString(other.m_Font));
                }

                return *this;
        }

        // Override the destructor
        ~ManagedFont() { ResourceManager::Get().Remove(m_Font); }

        // Used to retrive the Font from the class.
        const sf::Font* GetFont() const { return m_Font; }

       private:
        sf::Font* m_Font;
};
}  // namespace Engine
