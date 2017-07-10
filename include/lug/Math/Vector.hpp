#pragma once

#include <cstdint>
#include <lug/Math/Matrix.hpp>

namespace lug {
namespace Math {

// TODO: Redefine all operations on Matrix for Vector

template <uint8_t Rows, typename T = float>
class Vector : public Matrix<Rows, 1, T> {
public:
    using BaseMatrix = Matrix<Rows, 1, T>;

    constexpr Vector() = default;

    explicit constexpr Vector(T value);
    Vector(std::initializer_list<T> list);

    // Convert from matrix (we want non explicit conversion)
    Vector(const BaseMatrix& matrix);
    Vector(BaseMatrix&& matrix);

    Vector(const Vector<Rows - 1, T>& vector, T value = 0);
    Vector(const Vector<Rows + 1, T>& vector);

    Vector(const Vector<Rows, T>& vector) = default;
    Vector(Vector<Rows, T>&& vector) = default;

    Vector<Rows, T>& operator=(const Vector<Rows, T>& vector) = default;
    Vector<Rows, T>& operator=(Vector<Rows, T>&& vector) = default;

    ~Vector() = default;

    Vector<Rows, T> operator*=(const Matrix<Rows, Rows, T>& rhs);

    Vector<Rows, T> operator*=(const Vector<Rows, T>& rhs);
    Vector<Rows, T> operator/=(const Vector<Rows, T>& rhs);

#define DEFINE_ACCESS(name, minimum_rows)                                                                               \
    template <bool EnableBool = true, typename = typename std::enable_if<(Rows >= minimum_rows) && EnableBool>::type>   \
    const T& name() const {                                                                                             \
        return (*this)(minimum_rows - 1);                                                                               \
    }                                                                                                                   \
                                                                                                                        \
    template <bool EnableBool = true, typename = typename std::enable_if<(Rows >= minimum_rows) && EnableBool>::type>   \
    T& name() {                                                                                                         \
        return (*this)(minimum_rows - 1);                                                                               \
    }

    DEFINE_ACCESS(x, 1)
    DEFINE_ACCESS(r, 1)
    DEFINE_ACCESS(width, 1)

    DEFINE_ACCESS(y, 2)
    DEFINE_ACCESS(g, 2)
    DEFINE_ACCESS(height, 2)

    DEFINE_ACCESS(z, 3)
    DEFINE_ACCESS(b, 3)

    DEFINE_ACCESS(w, 4)
    DEFINE_ACCESS(a, 4)

#undef DEFINE_ACCESS

    constexpr T length() const;
    constexpr T squaredLength() const;
    void normalize();
};

template <typename T>
constexpr Vector<3, T> cross(const Vector<3, T>& lhs, const Vector<3, T>& rhs);

template <uint8_t Rows, typename T>
constexpr T dot(const Vector<Rows, T>& lhs, const Vector<Rows, T>& rhs);

template <uint8_t Rows, uint8_t Columns, typename T>
constexpr Matrix<Rows, Columns, T> outer(const Vector<Rows, T>& lhs, const Vector<Columns, T>& rhs);

template <uint8_t Rows, typename T>
constexpr Vector<Rows, T> normalize(const Vector<Rows, T>& lhs);

#define DEFINE_LENGTH_VECTOR(length)                        \
    template <typename T = float>                           \
    using Vec##length = Vector<length, T>;                  \
                                                            \
    template class LUG_MATH_API Vector<length, float>;      \
    using Vec##length##f = Vec##length<float>;              \
                                                            \
    template class LUG_MATH_API Vector<length, double>;     \
    using Vec##length##d = Vec##length<double>;             \
                                                            \
    template class LUG_MATH_API Vector<length, int32_t>;    \
    using Vec##length##i = Vec##length<int32_t>;            \
                                                            \
    template class LUG_MATH_API Vector<length, uint32_t>;   \
    using Vec##length##u = Vec##length<uint32_t>;

DEFINE_LENGTH_VECTOR(2)
DEFINE_LENGTH_VECTOR(3)
DEFINE_LENGTH_VECTOR(4)

#undef DEFINE_LENGTH_VECTOR

template <uint8_t Rows, typename T>
Vector<Rows, T> operator*(const Vector<Rows, T>& lhs, const Vector<Rows, T>& rhs);

template <uint8_t Rows, typename T>
Vector<Rows, T> operator/(const Vector<Rows, T>& lhs, const Vector<Rows, T>& rhs);

template <uint8_t Rows, typename T>
Vector<Rows, T> operator*(const Vector<Rows, T>& lhs, const Matrix<Rows, Rows, T>& rhs);

template <uint8_t Rows, typename T>
Vector<Rows, T> operator*(const Matrix<Rows, Rows, T>& lhs, const Vector<Rows, T>& rhs);

// Special case Mat4x4 * Vec3
template <typename T>
Vector<3, T> operator*(const Vector<3, T>& lhs, const Matrix<4, 4, T>& rhs);

template <typename T>
Vector<3, T> operator*(const Matrix<4, 4, T>& lhs, const Vector<3, T>& rhs);

#include <lug/Math/Vector.inl>

} // Math
} // lug
