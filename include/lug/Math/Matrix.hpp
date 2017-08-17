#pragma once

#include <cstdint>
#include <valarray>
#include <lug/Math/Export.hpp>
#include <lug/Math/ValArray.hpp>
#include <lug/System/Debug.hpp>

namespace lug {
namespace Math {

template <uint8_t Rows, uint8_t Columns, typename T = float>
class Matrix {
public:
    // TODO: Use custom valarray with compile time size
    using Values = ValArray<Rows * Columns, T>;

public:
    constexpr Matrix() = default;

    explicit Matrix(T value);
    Matrix(const Values& values);
    Matrix(std::initializer_list<T> list);
    Matrix(const Matrix<Rows, Columns, T>& matrix) = default;
    Matrix(Matrix<Rows, Columns, T>&& matrix) = default;

    Matrix<Rows, Columns, T>& operator=(const Matrix<Rows, Columns, T>& rhs) = default;
    Matrix<Rows, Columns, T>& operator=(Matrix<Rows, Columns, T>&& rhs) = default;

    ~Matrix() = default;

    constexpr uint8_t getRows() const;
    constexpr uint8_t getColumns() const;

    Values& getValues();
    constexpr const Values& getValues() const;

    T& operator()(uint8_t row, uint8_t col = 0);
    constexpr const T& operator()(uint8_t row, uint8_t col = 0) const;

    // Matrix/Scalar operations
    Matrix<Rows, Columns, T>& operator+=(T rhs);
    Matrix<Rows, Columns, T>& operator-=(T rhs);
    Matrix<Rows, Columns, T>& operator*=(T rhs);
    Matrix<Rows, Columns, T>& operator/=(T rhs);

    // Matrix/Matrix operations
    Matrix<Rows, Columns, T>& operator+=(const Matrix<Rows, Columns, T>& rhs);
    Matrix<Rows, Columns, T>& operator-=(const Matrix<Rows, Columns, T>& rhs);

#if defined(LUG_COMPILER_MSVC)

    template <typename = typename std::enable_if<(Rows == Columns)>::type>
    Matrix<Rows, Columns, T>& operator*=(const Matrix<Rows, Columns, T>& rhs);

    template <typename = typename std::enable_if<(Rows == Columns)>::type>
    Matrix<Rows, Columns, T>& operator/=(const Matrix<Rows, Columns, T>& rhs);

#else

    template <bool EnableBool = true>
    typename std::enable_if<(Rows == Columns) && EnableBool, Matrix<Rows, Columns, T>>::type& operator*=(const Matrix<Rows, Columns, T>& rhs);

    template <bool EnableBool = true>
    typename std::enable_if<(Rows == Columns) && EnableBool, Matrix<Rows, Columns, T>>::type& operator/=(const Matrix<Rows, Columns, T>& rhs);

#endif

#if defined(LUG_COMPILER_MSVC)

    template <typename = typename std::enable_if<(Rows == 1)>::type>
    Matrix<Rows, Columns, T> inverse() const;

    template <typename = typename std::enable_if<(Rows == 2)>::type, typename = void>
    Matrix<Rows, Columns, T> inverse() const;

    template <typename = typename std::enable_if<(Rows == 3)>::type, typename = void, typename = void>
    Matrix<Rows, Columns, T> inverse() const;

    template <typename = typename std::enable_if<(Rows == 4)>::type, typename = void, typename = void, typename = void>
    Matrix<Rows, Columns, T> inverse() const;

#else

    template <bool EnableBool = true>
    typename std::enable_if<(Rows == 1) && EnableBool, Matrix<Rows, Columns, T>>::type inverse() const;

    template <bool EnableBool = true>
    typename std::enable_if<(Rows == 2) && EnableBool, Matrix<Rows, Columns, T>>::type inverse() const;

    template <bool EnableBool = true>
    typename std::enable_if<(Rows == 3) && EnableBool, Matrix<Rows, Columns, T>>::type inverse() const;

    template <bool EnableBool = true>
    typename std::enable_if<(Rows == 4) && EnableBool, Matrix<Rows, Columns, T>>::type inverse() const;

#endif

    Matrix<Columns, Rows, T> transpose() const;

#if defined(LUG_COMPILER_MSVC)

    template <typename = typename std::enable_if<(Rows == 1)>::type>
    T det() const;

    template <typename = typename std::enable_if<(Rows == 2)>::type, typename = void>
    T det() const;

    template <typename = typename std::enable_if<(Rows == 3)>::type, typename = void, typename = void>
    T det() const;

    template <typename = typename std::enable_if<(Rows == 4)>::type, typename = void, typename = void, typename = void>
    T det() const;

    template <typename = typename std::enable_if<(Rows > 4)>::type, typename = void, typename = void, typename = void, typename = void>
    T det() const;

#else

    template <bool EnableBool = true>
    typename std::enable_if<(Rows == 1) && EnableBool, T>::type det() const;

    template <bool EnableBool = true>
    typename std::enable_if<(Rows == 2) && EnableBool, T>::type det() const;

    template <bool EnableBool = true>
    typename std::enable_if<(Rows == 3) && EnableBool, T>::type det() const;

    template <bool EnableBool = true>
    typename std::enable_if<(Rows == 4) && EnableBool, T>::type det() const;

    template <bool EnableBool = true>
    typename std::enable_if<(Rows > 4) && EnableBool, T>::type det() const;

#endif

#if defined(LUG_COMPILER_MSVC)

    template <typename = typename std::enable_if<(Rows == Columns)>::type>
    static Matrix<Rows, Columns, T> identity();

#else

    template <bool EnableBool = true>
    static typename std::enable_if<(Rows == Columns) && EnableBool, Matrix<Rows, Columns, T>>::type identity();

#endif

protected:
    Values _values{};
};

#define DEFINE_LENGTH_MATRIX(rows, columns)                             \
    template <typename T = float>                                       \
    using Mat##rows##x##columns= Matrix<rows, columns, T>;              \
                                                                        \
    template class LUG_MATH_API Matrix<rows, columns, float>;           \
    using Mat##rows##x##columns##f = Mat##rows##x##columns<float>;      \
                                                                        \
    template class LUG_MATH_API Matrix<rows, columns, double>;          \
    using Mat##rows##x##columns##d = Mat##rows##x##columns<double>;     \
                                                                        \
    template class LUG_MATH_API Matrix<rows, columns, int32_t>;         \
    using Mat##rows##x##columns##i = Mat##rows##x##columns<int32_t>;    \
                                                                        \
    template class LUG_MATH_API Matrix<rows, columns, uint32_t>;        \
    using Mat##rows##x##columns##u = Mat##rows##x##columns<uint32_t>;

DEFINE_LENGTH_MATRIX(2, 2)
DEFINE_LENGTH_MATRIX(2, 3)
DEFINE_LENGTH_MATRIX(2, 4)
DEFINE_LENGTH_MATRIX(3, 2)
DEFINE_LENGTH_MATRIX(3, 3)
DEFINE_LENGTH_MATRIX(3, 4)
DEFINE_LENGTH_MATRIX(4, 2)
DEFINE_LENGTH_MATRIX(4, 3)
DEFINE_LENGTH_MATRIX(4, 4)

#undef DEFINE_LENGTH_MATRIX

// Unary operations
template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T> operator-(const Matrix<Rows, Columns, T>& lhs);

// Matrix/Scalar operations
template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T> operator+(const Matrix<Rows, Columns, T>& lhs, T rhs);

template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T> operator-(const Matrix<Rows, Columns, T>& lhs, T rhs);

template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T> operator*(const Matrix<Rows, Columns, T>& lhs, T rhs);

template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T> operator/(const Matrix<Rows, Columns, T>& lhs, T rhs);

template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T> operator+(T lhs, const Matrix<Rows, Columns, T>& rhs);

template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T> operator-(T lhs, const Matrix<Rows, Columns, T>& rhs);

template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T> operator*(T lhs, const Matrix<Rows, Columns, T>& rhs);

template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T> operator/(T lhs, const Matrix<Rows, Columns, T>& rhs);

// Matrix/Matrix operation
template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T> operator+(const Matrix<Rows, Columns, T>& lhs, const Matrix<Rows, Columns, T>& rhs);

template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T> operator-(const Matrix<Rows, Columns, T>& lhs, const Matrix<Rows, Columns, T>& rhs);

template <uint8_t RowsLeft, uint8_t ColumnsLeft, uint8_t RowsRight, uint8_t ColumnsRight, typename T>
Matrix<RowsLeft, ColumnsRight, T> operator*(const Matrix<RowsLeft, ColumnsLeft, T>& lhs, const Matrix<RowsRight, ColumnsRight, T>& rhs);

template <uint8_t RowsLeft, uint8_t ColumnsLeft, uint8_t RowsRight, uint8_t ColumnsRight, typename T>
Matrix<RowsLeft, ColumnsRight, T> operator/(const Matrix<RowsLeft, ColumnsLeft, T>& lhs, const Matrix<RowsRight, ColumnsRight, T>& rhs);

// Comparaison operators
template <uint8_t Rows, uint8_t Columns, typename T>
bool operator==(const Matrix<Rows, Columns, T>& lhs, const Matrix<Rows, Columns, T>& rhs);

template <uint8_t Rows, uint8_t Columns, typename T>
bool operator!=(const Matrix<Rows, Columns, T>& lhs, const Matrix<Rows, Columns, T>& rhs);

template <uint8_t Rows, uint8_t Columns, typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<Rows, Columns, T>& matrix);

#include <lug/Math/Matrix.inl>

} // Math
} // lug
