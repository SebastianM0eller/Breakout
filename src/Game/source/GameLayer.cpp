#include "GameLayer.h"

#include "Engine/Renderer.h"
#include "Engine/ResourceManager.h"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Window/Keyboard.hpp"

GameLayer::GameLayer()
    : m_TestSprite(*Engine::ResourceManager::Get().Load<sf::Texture>("assets/TornadoPressure.png")) {}

GameLayer::~GameLayer() { Engine::ResourceManager::Get().Remove<sf::Texture>("assets/TornadoPressure.png"); }

void GameLayer::OnUpdate(float deltaTime) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) m_TestSprite.move({0, -10 * deltaTime});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) m_TestSprite.move({0, 10 * deltaTime});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) m_TestSprite.move({-10 * deltaTime, 0});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) m_TestSprite.move({10 * deltaTime, 0});
}

void GameLayer::OnRender() { Engine::Renderer::Get().Draw(m_TestSprite); }
