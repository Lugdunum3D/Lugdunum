#pragma once

#include <cmath>
#include <limits>

namespace lug {
namespace Math {
namespace Geometry {

template <typename T = double>
T radians(T degrees);

template <typename T = double>
T degrees(T radians);

template <typename T = double>
T sin(T radians);

template <typename T = double>
T cos(T radians);

template <typename T = double>
T tan(T radians);

template <typename T = double>
T asin(T radians);

template <typename T = double>
T acos(T radians);

template <typename T = double>
T atan(T radians);

template <typename T = double>
T atan2(T y, T x);

#include <lug/Math/Geometry/Trigonometry.inl>

} // Geometry
} // Math
} // lug
