#pragma once

#include "SFML/Graphics/RenderWindow.hpp"

namespace Engine {

class Application {
       public:
        struct ApplicationConfig {
                const char* ApplicationName{"Game"};
                sf::Vector2f WindowSize{800, 600};
                bool vsync{true};
        };

        Application(const ApplicationConfig& config);
        ~Application() = default;

        void Run();
        void Stop();

       private:
        sf::RenderWindow m_Window;
        bool m_IsRunning;
};

}  // namespace Engine
