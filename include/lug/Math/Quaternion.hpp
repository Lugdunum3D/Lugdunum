#pragma once

#include <lug/Math/Export.hpp>
#include <lug/Math/Matrix.hpp>
#include <lug/Math/Vector.hpp>
#include <lug/Math/Constant.hpp>

#include <cmath>

namespace lug {
namespace Math {

template <typename T = double>
class Quaternion {
public:
    Quaternion() = default;
    Quaternion(T w, T x, T y, T z);
    Quaternion(T data[4]);
    Quaternion(T angle, const Vector<3, T>& axis);

    Quaternion(const Quaternion<T>&) = default;
    Quaternion(Quaternion<T>&&) = default;

    Quaternion<T>& operator=(const Quaternion<T>&) = default;
    Quaternion<T>& operator=(Quaternion<T>&&) = default;

    ~Quaternion() = default;

    T& operator[](std::size_t idx);
    const T& operator[](std::size_t idx) const;

    void conjugate();
    void inverse();

    void normalize();
    constexpr T length() const;
    constexpr T squaredLength() const;

    T getAngle() const;
    Vector<3, T> getAxis() const;

    Mat4x4<T> transform() const;

#define DEFINE_QUATERNION_ACCESS(name, rows)    \
    const T& name() const {                     \
        return (*this)[rows];                   \
    }                                           \
                                                \
    T& name() {                                 \
        return (*this)[rows];                   \
    }

    DEFINE_QUATERNION_ACCESS(w, 0)
    DEFINE_QUATERNION_ACCESS(i, 0)

    DEFINE_QUATERNION_ACCESS(x, 1)
    DEFINE_QUATERNION_ACCESS(j, 1)

    DEFINE_QUATERNION_ACCESS(y, 2)
    DEFINE_QUATERNION_ACCESS(k, 2)

    DEFINE_QUATERNION_ACCESS(z, 3)
    DEFINE_QUATERNION_ACCESS(l, 3)

#undef DEFINE_QUATERNION_ACCESS

    static Quaternion<T> identity();

    static Quaternion<T> fromAxes(const Vector<3, T>& xAxis, const Vector<3, T>& yAxis, const Vector<3, T>& zAxis);
    static Quaternion<T> fromRotationMatrix(const Matrix<4, 4, T>& rotMatrix);

private:
    T _data[4];
};

template class LUG_MATH_API Quaternion<float>;
using Quatf = Quaternion<float>;

template class LUG_MATH_API Quaternion<double>;
using Quatd = Quaternion<double>;

template <typename T>
Quaternion<T> normalize(const Quaternion<T>& lhs);

template <typename T>
Quaternion<T> conjugate(const Quaternion<T>& lhs);

template <typename T>
Quaternion<T> inverse(const Quaternion<T>& lhs);

template <typename T>
T dot(const Quaternion<T>& lhs, const Quaternion<T>& rhs);

template <typename T>
Quaternion<T> directionTo(const Vector<3, T>& original, const Vector<3, T>& expected);

// TODO: Reflection / rotation

// Quaternion operator

template <typename T>
Quaternion<T> operator-(const Quaternion<T>& lhs);

// Quaternion/Quaternion operator
template <typename T>
Quaternion<T> operator+(const Quaternion<T>& lhs, const Quaternion<T>& rhs);

template <typename T>
Quaternion<T> operator-(const Quaternion<T>& lhs, const Quaternion<T>& rhs);

template <typename T>
Quaternion<T> operator*(const Quaternion<T>& lhs, const Quaternion<T>& rhs);

template <typename T>
Quaternion<T> operator/(const Quaternion<T>& lhs, const Quaternion<T>& rhs);

template <typename T>
bool operator==(const Quaternion<T>& lhs, const Quaternion<T>& rhs);

template <typename T>
bool operator!=(const Quaternion<T>& lhs, const Quaternion<T>& rhs);

template <typename T>
std::ostream& operator<<(std::ostream& os, const Quaternion<T>& quaternion);

#include  <lug/Math/Quaternion.inl>

} // Math
} // lug
