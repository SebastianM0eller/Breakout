#pragma once

#include <Engine/ECS/System.h>
#include <Engine/Renderer.h>

#include "BreakoutECS.h"
#include "Components.h"
namespace Breakout {
class RenderSystem : public Engine::System {
       public:
        void OnRender(BreakoutECS& system) {
                for (auto const entity : m_Entities) {
                        Breakout::Sprite& sprite = system.template GetComponent<Breakout::Sprite>(entity);
                        Engine::Renderer::Get().Draw(sprite.sprite.GetSprite());
                }
        }

        static void RegisterSelf(BreakoutECS& system) {
                BreakoutSignature signature;
                signature.set(system.GetComponentType<Breakout::Sprite>(), true);

                system.RegisterSystem<RenderSystem>();
                system.SetSystemSignature<RenderSystem>(signature);
        }
};
}  // namespace Breakout
