#pragma once

#include "SFML/Graphics/RenderWindow.hpp"

namespace Engine {

class Application {
       public:
        ///
        /// Represents the configuration settings for the Application.
        /// It is used to construct a new Application instance.
        ///
        struct ApplicationConfig {
                const char* WindowName{"Game"};
                sf::Vector2f WindowViewSize{800, 600};
                bool vsync{true};
        };

        Application(const ApplicationConfig& config);
        ~Application() = default;

        void Run();
        void Stop();

       private:
        sf::RenderWindow* m_Window;
        bool m_IsRunning;
};
}  // namespace Engine
