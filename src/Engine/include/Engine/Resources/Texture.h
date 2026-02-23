#pragma once

#include "ResourceManager.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
namespace Engine {
class ManagedSprite {
       public:
        // Used for default initialization, needed for ECS.
        ManagedSprite() : m_Sprite(*ResourceManager::Get().Load<sf::Texture>("assets/Texture/Fallback.png")) {}

        // Used to load a specific sprite.
        ManagedSprite(const std::string& sourcePath)
            : m_Sprite(*ResourceManager::Get().Load<sf::Texture>(sourcePath)) {};

        // Override the default copy.
        ManagedSprite(const ManagedSprite& other)
            : m_Sprite(
                  *ResourceManager::Get().Load<sf::Texture>(ResourceManager::Get().GetString(other.GetTexture()))) {};

        // Override the assignment operator.
        ManagedSprite& operator=(const ManagedSprite& other) {
                if (this != &other) {
                        ResourceManager& instance = ResourceManager::Get();

                        // Decrease the count for the initial Texture
                        instance.Remove(GetTexture());

                        sf::Texture* newTexture = instance.Load<sf::Texture>(instance.GetString(other.GetTexture()));
                        m_Sprite.setTexture(*newTexture, true);
                }
                return *this;
        }

        // Override the destructor
        ~ManagedSprite() { ResourceManager::Get().Remove(GetTexture()); }

        sf::Sprite& GetSprite() { return m_Sprite; }

       private:
        sf::Sprite m_Sprite;
        sf::Texture* GetTexture() const { return const_cast<sf::Texture*>(&m_Sprite.getTexture()); }
};
}  // namespace Engine
