#pragma once
#include <cassert>

#include "SFML/Graphics/RenderWindow.hpp"

namespace Engine {
class Renderer {
       public:
        Renderer(const Renderer&) = delete;

        ///
        /// Retrives the Renderer instance.
        /// If the Renderer is not yet initialized, it's constructed automatically.
        /// The first time you get the Renderer, you need to provide it with a window before it can draw.
        ///
        static Renderer& Get() {
                static Renderer instance;
                return instance;
        }

        ///
        /// Sets the internal window to the provided window.
        /// Asserts that the window is a valid ptr.
        ///
        void SetWindow(sf::RenderWindow* window) {
                assert(window);
                m_Window = window;
        }

        ///
        /// Draws the object to the current RenderWindow.
        /// Asserts that the RenderWindow is a valid ptr.
        ///
        template <typename T>
        void Draw(T Object) {
                assert(m_Window);
                m_Window->draw(Object);
        }

        ///
        /// Clears the current frame buffer.
        /// Should be called before drawing or right after Display.
        ///
        void Clear() {
                assert(m_Window);
                m_Window->clear();
        }

        ///
        /// Displays the current buffer to the screen.
        /// Should be called after drawing has finished, and before Clear.
        ///
        void Display() {
                assert(m_Window);
                m_Window->display();
        }

       private:
        Renderer() {}

        sf::RenderWindow* m_Window{nullptr};
};
}  // namespace Engine
