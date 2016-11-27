#pragma once

#include <limits>

namespace lug {
namespace Math {
namespace Geometry {

// TODO: Add cos, etc

template <typename T = double>
T radians(T degrees);

template <typename T = double>
T degrees(T radians);

#include <lug/Math/Geometry/Trigonometry.inl>

}
}
}
