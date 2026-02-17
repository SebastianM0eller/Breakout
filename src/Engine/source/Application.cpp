#include "Engine/Application.h"

Engine::Application::Application(const ApplicationConfig& config) {
        m_Window.create(sf::VideoMode::getDesktopMode(), config.ApplicationName);
        m_Window.setView(sf::View(sf::FloatRect({0, 0}, config.WindowSize)));

        m_IsRunning = false;
}

void Engine::Application::Run() {
        m_IsRunning = true;

        while (m_IsRunning) {
                while (const std::optional event = m_Window.pollEvent()) {
                        if (event->is<sf::Event::Closed>()) {
                                Stop();
                        }
                }

                m_Window.display();
        }
}

void Engine::Application::Stop() { m_IsRunning = false; }
