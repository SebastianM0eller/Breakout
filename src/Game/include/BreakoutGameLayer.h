#pragma once

#include <Engine/ECS/ECS.h>
#include <Engine/Layer.h>

#include "BreakoutECS.h"

class BreakoutGameLayer : public Engine::Layer {
   public:
    BreakoutGameLayer();
    ~BreakoutGameLayer();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;

   private:
    BreakoutECS m_ECS;

    void RegisterComponents();
    void RegisterSystems();
    void RegisterEntities();
    void LoadAssets();
    void UnloadAssets();
};
