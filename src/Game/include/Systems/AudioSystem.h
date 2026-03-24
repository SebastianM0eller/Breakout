#pragma once

#include <Engine/ECS/System.h>
#include <Engine/Resources/ResourceManager.h>

#include <iostream>
#include <memory>

#include "BreakoutECS.h"
#include "Events.h"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/System/Clock.hpp"

class GameSound : public sf::Sound {
   public:
    using sf::Sound::Sound;  // For the constructor.
    sf::Clock lifeTime;      // To track the lifeTime.
};

namespace Breakout {
class AudioSystem : public Engine::System {
   public:
    void PlaySound(const PlaySoundEvent& sound) {
        sf::SoundBuffer* buffer =
            Engine::ResourceManager::Get().Load<sf::SoundBuffer>(sound.path);

        // If the buffer is invalid, we should not play it.
        // Should be logged.
        if (!buffer)
            return;

        for (auto& s : m_ActiveSounds) {
            if (s.getStatus() != sf::Sound::Status::Playing &&
                s.lifeTime.getElapsedTime().asSeconds() > 0.5f) {
                s.setBuffer(*buffer);
                s.play();

                Engine::ResourceManager::Get().Remove(buffer);
                return;
            }
        }

        m_ActiveSounds.emplace_back(*buffer);
        m_ActiveSounds.back().play();

        Engine::ResourceManager::Get().Remove(buffer);
    }

    static void RegisterSelf(BreakoutECS& system) {
        auto self = system.RegisterSystem<AudioSystem>();

        // Allocate the sf::Sound device, to avoid stutter.
        self->m_ActiveSounds.reserve(16);

        std::weak_ptr<AudioSystem> weakself = self;

        system.AddListner<PlaySoundEvent>(
            [weakself](const PlaySoundEvent& event) {
                if (auto lockedSelf = weakself.lock()) {
                    lockedSelf->PlaySound(event);
                }
            });
    }

   private:
    std::vector<GameSound> m_ActiveSounds;
};
}  // namespace Breakout
