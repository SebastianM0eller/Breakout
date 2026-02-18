#pragma once

#include "SFML/Window/Event.hpp"
namespace Engine {
class Layer {
       public:
        virtual ~Layer() = default;

        virtual bool OnEvent([[maybe_unused]] sf::Event& event) { return false; }
        virtual void OnUpdate([[maybe_unused]] float deltaTime) {}
        virtual void OnRender() {}
};
}  // namespace Engine
