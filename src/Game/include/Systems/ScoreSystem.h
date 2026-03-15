#pragma once

#include <Engine/ECS/System.h>
#include <Engine/Renderer.h>

#include <string>

#include "BreakoutECS.h"
#include "Components.h"
#include "Events.h"

namespace Breakout {
class ScoreSystem : public Engine::System {
       public:
        void OnScoreIncreased(const ScoreIncreasedEvent& event, BreakoutECS& system) {
                for (const auto entity : m_Entities) {
                        Score& score = system.GetComponent<Score>(entity);
                        score.score += event.increase;
                }
        }

        void OnRender(BreakoutECS& system) {
                for (const auto entity : m_Entities) {
                        // Get the data.
                        Text& text = system.GetComponent<Text>(entity);
                        Score& score = system.GetComponent<Score>(entity);
                        Transform& transform = system.GetComponent<Transform>(entity);

                        text.text.SetString(std::to_string(score.score));  // Set the correct score.
                        text.text.CenterOrigin();                          // Center the origin.
                        text.text.SetPosition(transform.location);         // Set the origin position.

                        Engine::Renderer::Get().Draw(text.text.GetText());
                }
        }

        static void RegisterSelf(BreakoutECS& system) {
                BreakoutSignature signature;
                signature.set(system.GetComponentType<Score>(), true);
                signature.set(system.GetComponentType<Text>(), true);
                signature.set(system.GetComponentType<Transform>(), true);

                auto self = system.RegisterSystem<ScoreSystem>();
                system.SetSystemSignature<ScoreSystem>(signature);

                std::weak_ptr<ScoreSystem> weakSelf = self;

                system.AddListner<ScoreIncreasedEvent>([weakSelf, &system](const ScoreIncreasedEvent& event) {
                        if (auto lockedSelf = weakSelf.lock()) {
                                lockedSelf->OnScoreIncreased(event, system);
                        }
                });
        };
};
}  // namespace Breakout
