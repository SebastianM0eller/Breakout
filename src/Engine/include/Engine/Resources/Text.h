#pragma once

#include "ResourceManager.h"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"
namespace Engine {

// Just a simple wrapper, to allow default for initialization.
class Text {
       public:
        Text() : m_Text(*ResourceManager::Get().Load<sf::Font>("assets/Fonts/DefaultFont.ttf"), "", 18) {};
        Text(uint8_t fs) : m_Text(*ResourceManager::Get().Load<sf::Font>("assets/Fonts/DefaultFont.ttf"), "", fs) {};
        Text(const sf::Text& other) : m_Text(other) {}

        void SetString(const std::string& string) { m_Text.setString(string); }
        void SetPosition(sf::Vector2f position) { m_Text.setPosition(position); }
        void CenterOrigin() { m_Text.setOrigin(m_Text.getGlobalBounds().size / 2.0f); }

        sf::Text GetText() const { return m_Text; }

       private:
        sf::Text m_Text;
};
}  // namespace Engine
