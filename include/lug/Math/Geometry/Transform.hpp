#pragma once

#include <lug/Math/Matrix.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Math {
namespace Geometry {

template <typename T>
Matrix<4, 4, T> translate(const Vector<3, T> &direction);

template <typename T>
Matrix<4, 4, T> rotate(T angle, const Vector<3, T> &axis);

template <typename T>
Matrix<4, 4, T> scale(const Vector<3, T> &factors);

#include <lug/Math/Geometry/Transform.inl>

}
}
}
