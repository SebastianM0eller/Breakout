#include <Engine/Application.h>
#include <Engine/ECS/ECS.h>

#include "BreakoutGameLayer.h"

int main() {
        Engine::Application::ApplicationConfig config;
        config.WindowViewSize = {400, 300};
        Engine::Application game(config);

        game.PushLayer<BreakoutGameLayer>();
        game.Run();
}
