#pragma once

#include <concepts>
#include <memory>
#include <vector>

#include "Engine/Layer.h"
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
                sf::Vector2f WindowViewSize{400, 300};
                bool vsync{true};
        };

        Application(const ApplicationConfig& config);

        void Run();
        void Stop();

        template <typename T>
                requires(std::derived_from<T, Layer>)
        void PushLayer() {
                m_LayerStack.push_back(std::make_unique<T>());
        }

       private:
        std::shared_ptr<sf::RenderWindow> m_Window{};
        std::vector<std::unique_ptr<Layer>> m_LayerStack;
        bool m_IsRunning;

        void HandleEvents();
};
}  // namespace Engine
