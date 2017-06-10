#pragma once

#define NOMINMAX
#include <algorithm>
#include <array>
#include <cstring>
#include <numeric>

namespace lug {
namespace Math {

template <size_t Size, typename T = float>
class ValArray {
public:
    ValArray() = default;
    explicit ValArray(const T& value);
    explicit ValArray(const T* values);
    ValArray(const ValArray<Size, T>& rhs) = default;
    ValArray(ValArray<Size, T>&& rhs) = default;
    ValArray(std::initializer_list<T> list);

    ValArray<Size, T>& operator=(const ValArray<Size, T>& rhs) = default;
    ValArray<Size, T>& operator=(ValArray<Size, T>&& rhs) = default;

    ~ValArray() = default;

    const T& operator[](size_t pos) const;
    T& operator[](size_t pos);

    const std::array<T, Size>& data() const;
    std::array<T, Size>& data();

    constexpr size_t size() const;

    T sum() const;

    // ValArray/Scalar operations
    ValArray<Size, T>& operator+=(const T& rhs);
    ValArray<Size, T>& operator-=(const T& rhs);
    ValArray<Size, T>& operator*=(const T& rhs);
    ValArray<Size, T>& operator/=(const T& rhs);

    // ValArray/ValArray operations
    ValArray<Size, T>& operator+=(const ValArray<Size, T>& rhs);
    ValArray<Size, T>& operator-=(const ValArray<Size, T>& rhs);
    ValArray<Size, T>& operator*=(const ValArray<Size, T>& rhs);
    ValArray<Size, T>& operator/=(const ValArray<Size, T>& rhs);

private:
    std::array<T, Size> _data;
};

// ValArray/Scalar operations
template <size_t Size, typename T = float>
ValArray<Size, T> operator+(const ValArray<Size, T>& lhs, const T& rhs);

template <size_t Size, typename T = float>
ValArray<Size, T> operator-(const ValArray<Size, T>& lhs, const T& rhs);

template <size_t Size, typename T = float>
ValArray<Size, T> operator*(const ValArray<Size, T>& lhs, const T& rhs);

template <size_t Size, typename T = float>
ValArray<Size, T> operator/(const ValArray<Size, T>& lhs, const T& rhs);

template <size_t Size, typename T = float>
ValArray<Size, T> operator+(const T& lhs, const ValArray<Size, T>& rhs);

template <size_t Size, typename T = float>
ValArray<Size, T> operator-(const T& lhs, const ValArray<Size, T>& rhs);

template <size_t Size, typename T = float>
ValArray<Size, T> operator*(const T& lhs, const ValArray<Size, T>& rhs);

template <size_t Size, typename T = float>
ValArray<Size, T> operator/(const T& lhs, const ValArray<Size, T>& rhs);

// ValArray/ValArray operations
template <size_t Size, typename T = float>
ValArray<Size, T> operator+(const ValArray<Size, T>& lhs, const ValArray<Size, T>& rhs);

template <size_t Size, typename T = float>
ValArray<Size, T> operator-(const ValArray<Size, T>& lhs, const ValArray<Size, T>& rhs);

template <size_t Size, typename T = float>
ValArray<Size, T> operator*(const ValArray<Size, T>& lhs, const ValArray<Size, T>& rhs);

template <size_t Size, typename T = float>
ValArray<Size, T> operator/(const ValArray<Size, T>& lhs, const ValArray<Size, T>& rhs);

template <size_t Size, typename T = float>
bool operator==(const ValArray<Size, T>& lhs, const ValArray<Size, T>& rhs);

template <size_t Size, typename T = float>
bool operator!=(const ValArray<Size, T>& lhs, const ValArray<Size, T>& rhs);

#include <lug/Math/ValArray.inl>

} // Math
} // lug
