#pragma once

#include <cstdint>
namespace Breakout {

struct LifeGainedEvent {};
struct LifeUsedEvent {};
struct SpawnBallEvent {};
struct GameOverEvent {};
struct BallDestroyedEvent {};

struct ScoreIncreasedEvent {
        uint32_t increase;
};

}  // namespace Breakout
