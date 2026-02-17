#include "Engine/Application.h"

int main() {
        Engine::Application::ApplicationConfig config;
        Engine::Application game{config};

        game.Run();
}
