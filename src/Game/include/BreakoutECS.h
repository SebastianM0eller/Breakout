#pragma once
#include <Engine/ECS/ECS.h>

#include <bitset>

#include "Engine/ECS/Entity.h"

using BreakoutECS = Engine::ECS<32, 512>;
using BreakoutSignature = std::bitset<32>;
using BreakoutEntity = Engine::Entity;
