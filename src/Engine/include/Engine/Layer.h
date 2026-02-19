#pragma once

#include "SFML/Window/Event.hpp"

namespace Engine {
class Layer {
       public:
        virtual ~Layer() = default;

        ///
        /// Processes the events.
        /// Returns a boolean based on wether the event was consumed or not.
        /// true: The event was consumed, and will not continue to the later layers.
        /// false: The event was not consumed, and will be passed along.
        ///
        virtual bool OnEvent([[maybe_unused]] const sf::Event&) { return false; }

        virtual void OnUpdate([[maybe_unused]] const float deltaTime) {}

        virtual void OnRender() {}
};
}  // namespace Engine
