#pragma once

#include <Engine/ECS/ECS.h>
#include <Engine/Layer.h>

#include <bitset>

class BreakoutGameLayer : public Engine::Layer {
       public:
        using Signature = std::bitset<32>;  // Needs to be the same as the Component Count for m_ECS.
        BreakoutGameLayer();
        ~BreakoutGameLayer() = default;

        void OnUpdate(float deltaTime) override;
        void OnRender() override;

       private:
        Engine::ECS<32, 512> m_ECS;  // <Component Count, Entity Count>

        void RegisterComponents();
        void RegisterSystems();
        void RegisterEntities();
};
