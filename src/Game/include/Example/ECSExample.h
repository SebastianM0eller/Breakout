#pragma once

#include <Engine/ECS/ECS.h>
#include <Engine/Layer.h>

class ECSLayer : public Engine::Layer {
       public:
        ECSLayer();
        ~ECSLayer() = default;
        void OnUpdate(const float deltaTime) override;

       private:
        Engine::ECS<32, 1024> m_ECS;  // Should basically be the only data the scene needs.
                                      // We just need to register the things we need.

        void RegisterComponents();
        void RegisterSystems();
        void RegisterEntities();
};
