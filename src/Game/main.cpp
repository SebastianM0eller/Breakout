#include <iostream>

#include "Engine/Application.h"
#include "Engine/ResourceManager.h"
#include "SFML/Graphics/Texture.hpp"

int main() {
        sf::Texture* testTexture = Engine::ResourceManager::Get().Load<sf::Texture>("assets/TornadoPressure.png");
        if (!testTexture)
                std::cout << "Failed to load the testTexture";
        else
                std::cout << "Loaded the Texture";

        Engine::Application::ApplicationConfig config;
        Engine::Application game{config};

        game.Run();
}
