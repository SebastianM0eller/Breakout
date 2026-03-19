#pragma once

namespace Engine::Math {

///
/// Linear interpolation between two values.
/// Finds a value at a specific percentage (blend) between the start and the target.
/// Uses standard linear interpolation:
/// start + (target - start) * blend
///
inline float Lerp(float start, float target, float blend) { return start + (target - start) * blend; }

///
/// Easing function used for smooth deacceleration.
/// Can be used to transform the time param in lerp.
/// Uses quadratic easing based on:
/// Eased_Time = 1 - (1 - t) (1 - t)
///
inline float EaseOutQuadratic(float time) { return time * (2.0f - time); }

}  // namespace Engine::Math
