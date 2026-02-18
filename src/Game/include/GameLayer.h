#pragma once
#include <Engine/Layer.h>

#include "SFML/Graphics/Sprite.hpp"

class GameLayer : public Engine::Layer {
       public:
        GameLayer();
        ~GameLayer();

        void OnRender() override;
        void OnUpdate(float deltaTime) override;

       private:
        sf::Sprite m_TestSprite;
};
