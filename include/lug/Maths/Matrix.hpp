#pragma once

#include <cstdint>
#include <valarray>
#include <lug/System/Debug.hpp>

namespace lug {
namespace Maths {

template <uint8_t Rows, uint8_t Columns, typename T = float>
class Matrix
{
public:
    using Values = std::valarray<T>;

public:
    explicit constexpr Matrix(T value = 0);
    explicit Matrix(std::initializer_list<T> list);
    Matrix(const Matrix<Rows, Columns, T>& matrix) = default;
    Matrix(Matrix<Rows, Columns, T>&& matrix) = default;

    Matrix<Rows, Columns, T>& operator=(const Matrix<Rows, Columns, T>& rhs) = default;
    Matrix<Rows, Columns, T>& operator=(Matrix<Rows, Columns, T>&& rhs) = default;

    virtual ~Matrix() = default;

    constexpr uint8_t getRows() const;
    constexpr uint8_t getColumns() const;

    Matrix<Rows, Columns, T>::Values& getValues();
    constexpr const Matrix<Rows, Columns, T>::Values& getValues() const;

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
    Matrix<Rows, Columns, T>& operator*=(const Matrix<Rows, Columns, T>& rhs);
    Matrix<Rows, Columns, T>& operator/=(const Matrix<Rows, Columns, T>& rhs) = delete;

    // TODO: Add the invert function
    Matrix<Columns, Rows, T> transpose() const;

    template <bool EnableBool = true>
    constexpr typename std::enable_if<Rows == 1 && EnableBool, T>::type det() const;

    template <bool EnableBool = true>
    constexpr typename std::enable_if<Rows == 2 && EnableBool, T>::type det() const;

    template <bool EnableBool = true>
    constexpr typename std::enable_if<Rows == 3 && EnableBool, T>::type det() const;

    template <bool EnableBool = true>
    typename std::enable_if<(Rows > 3) && EnableBool, T>::type det() const;

    static Matrix<Rows, Columns, T> identity();

protected:
    Values _values;
};

// Matrix/Scalar operations
template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T> operator+(const Matrix<Rows, Columns, T>& lhs, T rhs);

template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T> operator-(const Matrix<Rows, Columns, T>& lhs, T rhs);

template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T> operator*(const Matrix<Rows, Columns, T>& lhs, T rhs);

template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T> operator/(const Matrix<Rows, Columns, T>& lhs, T rhs);

// Matrix/Matrix operation
template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T> operator+(const Matrix<Rows, Columns, T>& lhs, const Matrix<Rows, Columns, T>& rhs);

template <uint8_t Rows, uint8_t Columns, typename T>
Matrix<Rows, Columns, T> operator-(const Matrix<Rows, Columns, T>& lhs, const Matrix<Rows, Columns, T>& rhs);

template <uint8_t RowsLeft, uint8_t ColumnsLeft, uint8_t RowsRight, uint8_t ColumnsRight, typename T>
Matrix<RowsLeft, ColumnsRight, T> operator*(const Matrix<RowsLeft, ColumnsLeft, T>& lhs, const Matrix<RowsRight, ColumnsRight, T>& rhs);

template <uint8_t RowsLeft, uint8_t ColumnsLeft, uint8_t RowsRight, uint8_t ColumnsRight, typename T>
Matrix<RowsLeft, ColumnsRight, T> operator/(const Matrix<RowsLeft, ColumnsLeft, T>& lhs, const Matrix<RowsRight, ColumnsRight, T>& rhs) = delete;

// Comparaison operators
template <uint8_t Rows, uint8_t Columns, typename T>
bool operator==(const Matrix<Rows, Columns, T>& lhs, const Matrix<Rows, Columns, T>& rhs);

template <uint8_t Rows, uint8_t Columns, typename T>
bool operator!=(const Matrix<Rows, Columns, T>& lhs, const Matrix<Rows, Columns, T>& rhs);

template <uint8_t Rows, uint8_t Columns, typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<Rows, Columns, T>& matrix);

#include <lug/Maths/Matrix.inl>

}
}
