#pragma once

#include "SFML/Window/Event.hpp"
namespace Engine {
class Layer {
       public:
        virtual ~Layer() = default;

        virtual bool OnEvent(sf::Event& event) { return false; }
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() {}
};
}  // namespace Engine
