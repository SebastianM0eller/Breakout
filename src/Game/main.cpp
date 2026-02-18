#include <iostream>

#include "Engine/ResourceManager.h"

int main() {
        for (int i = 0; i < 5; i++) {
                Engine::ResourceManager::Get().Load<sf::Texture>("assets/TornadoPressure.png");
        }

        for (int i = 0; i < 8; i++) {
                std::cout << "Iteration: " << i << "\n";
                Engine::ResourceManager::Get().Remove<sf::Texture>("assets/TornadoPressure.png");
        }
}
