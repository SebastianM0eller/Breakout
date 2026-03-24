#pragma once

#include <cstdint>
#include <string>

namespace Breakout {

struct LifeGainedEvent {};
struct LifeUsedEvent {};
struct SpawnBallEvent {};
struct GameOverEvent {};
struct BallDestroyedEvent {};
struct BoxDestroyedEvent {};

struct ScoreIncreasedEvent {
    uint32_t increase;
};

struct PlaySoundEvent {
    std::string path;
};

}  // namespace Breakout
