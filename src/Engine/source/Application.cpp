#include "Engine/Application.h"

#include <memory>

#include "Engine/Renderer.h"
#include "SFML/System/Clock.hpp"

///
/// Creates a new Application instance based on the specified ApplicationConfig.
///
Engine::Application::Application(const ApplicationConfig& config) {
        m_Window->create(sf::VideoMode::getDesktopMode(), config.WindowName);
        m_Window->setView(sf::View(sf::FloatRect({0, 0}, config.WindowViewSize)));

        m_IsRunning = false;
}

///
/// Sets m_IsRunning to true, and starts the game loop.
///
void Engine::Application::Run() {
        m_IsRunning = true;
        sf::Clock timer;

        while (m_IsRunning) {
                float deltaTime = timer.restart().asSeconds();

                // Handle events
                while (const std::optional event = m_Window->pollEvent()) {
                        if (event->is<sf::Event::Closed>()) {
                                Stop();
                        }
                }

                // Handle Update.
                for (const std::unique_ptr<Layer>& layer : m_LayerStack) layer->OnUpdate(deltaTime);

                // Handle Rendering.
                Renderer::Get().Clear();
                for (const std::unique_ptr<Layer>& layer : m_LayerStack) layer->OnRender();
                Renderer::Get().Display();
        }
}

///
/// Stops the main game loop, by setting m_IsRunning to false.
/// The game loop will finish it's current cycle.
///
void Engine::Application::Stop() { m_IsRunning = false; }
