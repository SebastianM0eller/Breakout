#include <Engine/Application.h>
#include <Engine/ECS/ECS.h>

#include "ECSExample.h"

int main() {
        Engine::Application::ApplicationConfig config;
        Engine::Application game(config);

        game.PushLayer<ECSLayer>();
        game.Run();
}
