#pragma once
#include <Engine/ECS/ECS.h>

#include <bitset>

using BreakoutECS = Engine::ECS<32, 512>;
using BreakoutSignature = std::bitset<32>;
