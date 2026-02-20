#pragma once

#include <Engine/ECS/ECS.h>
#include <Engine/Layer.h>

#include <bitset>

class ECSLayer : public Engine::Layer {
       public:
        using Signature = std::bitset<32>;  // The number is based on the amount of components.
        ECSLayer();
        ~ECSLayer() = default;

        void OnRender() override;
        void OnUpdate(const float deltaTime) override;

       private:
        Engine::ECS<32, 1024> m_ECS;  // Should basically be the only data the scene needs.
                                      // We just need to register the things we need.
        void RegisterComponents();
        void RegisterSystems();
        void RegisterEntities();
        void InitEntities();
};
