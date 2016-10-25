#pragma once

#include <cstdint>
#include <lug/Maths/Matrix.hpp>

namespace lug {
namespace Maths {

template <uint8_t Rows, typename T = float>
class Vector : public Matrix<Rows, 1, T> {
public:
    using BaseMatrix = Matrix<Rows, 1, T>;

    explicit constexpr Vector(T value = 0);
    Vector(std::initializer_list<T> list);

    // Convert from matrix (we want non explicit conversion)
    Vector(const BaseMatrix& matrix);
    Vector(BaseMatrix&& matrix);

    Vector(const Vector<Rows - 1, T>& vector, T value = 0);

    Vector(const Vector<Rows, T>& vector) = default;
    Vector(Vector<Rows, T>&& vector) = default;

    Vector<Rows, T>& operator=(const Vector<Rows, T>& vector) = default;
    Vector<Rows, T>& operator=(Vector<Rows, T>&& vector) = default;

    ~Vector() = default;

#define DEFINE_ACCESS(name, minimum_rows)                                                                           \
    template <bool EnableBool = true>                                                                               \
    constexpr inline typename std::enable_if<(Rows >= minimum_rows) && EnableBool, const T&>::type name() const {   \
        return (*this)(minimum_rows - 1);                                                                           \
    }                                                                                                               \
                                                                                                                    \
    template <bool EnableBool = true>                                                                               \
    constexpr inline typename std::enable_if<(Rows >= minimum_rows) && EnableBool, T&>::type name(){                \
        return (*this)(minimum_rows - 1);                                                                           \
    }

    DEFINE_ACCESS(x, 1)
    DEFINE_ACCESS(r, 1)

    DEFINE_ACCESS(y, 2)
    DEFINE_ACCESS(g, 2)

    DEFINE_ACCESS(z, 3)
    DEFINE_ACCESS(b, 3)

    DEFINE_ACCESS(w, 4)
    DEFINE_ACCESS(a, 4)

#undef DEFINE_ACCESS

    constexpr T norme() const;
    constexpr void normalize();
};

template <typename T>
constexpr Vector<3, T> cross(const Vector<3, T>& lhs, const Vector<3, T>& rhs);

template <uint8_t Rows, typename T>
constexpr T dot(const Vector<Rows, T>& lhs, const Vector<Rows, T>& rhs);

template <uint8_t Rows, uint8_t Columns, typename T>
constexpr Matrix<Rows, Columns, T> outer(const Vector<Rows, T>& lhs, const Vector<Columns, T>& rhs);

#define DEFINE_LENGTH_VECTOR(length)                \
    template <typename T = float>                   \
    using Vec##length = Vector<length, T>;          \
                                                    \
    using Vec##length##f = Vec##length<float>;      \
    using Vec##length##d = Vec##length<double>;     \
    using Vec##length##i = Vec##length<int32_t>;    \
    using Vec##length##u = Vec##length<uint32_t>;

DEFINE_LENGTH_VECTOR(2)
DEFINE_LENGTH_VECTOR(3)
DEFINE_LENGTH_VECTOR(4)

#undef DEFINE_LENGTH_VECTOR

#include <lug/Maths/Vector.inl>

}
}
