#pragma once

#include "ResourceManager.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
namespace Engine {
class ManagedSprite {
   public:
    // Used for default initialization, needed for ECS.
    ManagedSprite()
        : m_Sprite(*ResourceManager::Get().Load<sf::Texture>(
              "assets/Textures/Fallback.png")) {
        m_Sprite.setOrigin(m_Sprite.getLocalBounds().size / 2.0f);
    }

    // Used to load a specific sprite.
    ManagedSprite(const std::string& sourcePath)
        : m_Sprite(*ResourceManager::Get().Load<sf::Texture>(sourcePath)) {
        m_Sprite.setOrigin(m_Sprite.getLocalBounds().size / 2.0f);
    };

    // Override the default copy.
    ManagedSprite(const ManagedSprite& other)
        : m_Sprite(*ResourceManager::Get().Load<sf::Texture>(
              ResourceManager::Get().GetString(other.GetTexture()))) {};

    // Override the assignment operator.
    ManagedSprite& operator=(const ManagedSprite& other) {
        if (this == &other)
            return *this;

        ResourceManager& instance = ResourceManager::Get();

        // Decrease the count for the initial Texture
        instance.Remove(GetTexture());

        sf::Texture* newTexture =
            instance.Load<sf::Texture>(instance.GetString(other.GetTexture()));
        m_Sprite.setTexture(*newTexture, true);
        m_Sprite.setOrigin(m_Sprite.getLocalBounds().size / 2.0f);

        return *this;
    }

    // Override the destructor
    ~ManagedSprite() {
        ResourceManager::Get().Remove(GetTexture());
    }

    void SetPosition(const sf::Vector2f newPosition) {
        m_Sprite.setPosition(newPosition);
    }
    const sf::Sprite& GetSprite() {
        return m_Sprite;
    }

   private:
    sf::Sprite m_Sprite;
    sf::Texture* GetTexture() const {
        return const_cast<sf::Texture*>(&m_Sprite.getTexture());
    }
};
}  // namespace Engine
