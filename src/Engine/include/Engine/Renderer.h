#pragma once
#include <cassert>
#include <memory>

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"

namespace Engine {
class Renderer {
       public:
        Renderer(const Renderer&) = delete;

        ///
        /// Retrives the Renderer instance.
        /// If the Renderer is not yet initialized, it's constructed automatically.
        /// The first time you get the Renderer, you need to provide it with a window before it can draw.
        ///
        static Renderer& Get() noexcept {
                static Renderer instance;
                return instance;
        }

        ///
        /// Sets the internal window to the provided window.
        /// Asserts that the window is a valid ptr.
        ///
        void SetWindow(std::shared_ptr<sf::RenderWindow> window) noexcept {
                assert(window && "Window is not allowed to be a nullptr");
                m_Window = window;
        }

        ///
        /// Draws the object to the current RenderWindow.
        /// Asserts that the RenderWindow is a valid ptr.
        ///
        void Draw(const sf::Drawable& object) {
                assert(m_Window && "Window has to be initialized");
                m_Window->draw(object);
        }

        ///
        /// Clears the current frame buffer.
        /// Should be called before drawing or right after Display.
        ///
        void Clear() {
                assert(m_Window && "Window has to be initialized");
                m_Window->clear();
        }

        ///
        /// Displays the current buffer to the screen.
        /// Should be called after drawing has finished, and before Clear.
        ///
        void Display() {
                assert(m_Window && "Window has to be initialized");
                m_Window->display();
        }

        ///
        /// Return a vector2 containing the size of the current windows ViewSize.
        /// It is returned as width, height.
        /// An assert is triggered if the window has not yet been initialized.
        ///
        sf::Vector2f GetViewSize() {
                assert(m_Window && "Wnidow has to be initialized");
                return m_Window->getView().getSize();
        }

       private:
        Renderer() {}

        std::shared_ptr<sf::RenderWindow> m_Window{nullptr};
};
}  // namespace Engine
