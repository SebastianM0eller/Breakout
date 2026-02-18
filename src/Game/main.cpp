#include <Engine/Application.h>

#include "GameLayer.h"

int main() {
        Engine::Application::ApplicationConfig config;
        Engine::Application game(config);

        game.PushLayer<GameLayer>();
        game.Run();
}
