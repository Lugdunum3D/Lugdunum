template <size_t Size, typename T>
ValArray<Size, T>::ValArray(const T& value) {
    _data.fill(value);
}

template <size_t Size, typename T>
ValArray<Size, T>::ValArray(const T* values) {
    std::memcpy(_data.data(), values, sizeof(T) * Size);
}

template <size_t Size, typename T>
ValArray<Size, T>::ValArray(std::initializer_list<T> list) {
    std::copy(std::begin(list), std::end(list), std::begin(_data));
}

template <size_t Size, typename T>
inline const T& ValArray<Size, T>::operator[](size_t pos) const {
    return _data[pos];
}

template <size_t Size, typename T>
inline T& ValArray<Size, T>::operator[](size_t pos) {
    return _data[pos];
}

template <size_t Size, typename T>
inline const std::array<T, Size>& ValArray<Size, T>::data() const {
    return _data;
}

template <size_t Size, typename T>
inline std::array<T, Size>& ValArray<Size, T>::data() {
    return _data;
}

template <size_t Size, typename T>
inline constexpr size_t ValArray<Size, T>::size() const {
    return Size;
}

template <size_t Size, typename T>
inline T ValArray<Size, T>::sum() const {
    return std::accumulate(std::begin(_data), std::end(_data), T(0));
}

// ValArray/Scalar operations
template <size_t Size, typename T>
inline ValArray<Size, T>& ValArray<Size, T>::operator+=(const T& rhs) {
    for (size_t i = 0; i < Size; ++i) {
        _data[i] += rhs;
    }

    return *this;
}

template <size_t Size, typename T>
inline ValArray<Size, T>& ValArray<Size, T>::operator-=(const T& rhs) {
    for (size_t i = 0; i < Size; ++i) {
        _data[i] -= rhs;
    }

    return *this;
}

template <size_t Size, typename T>
inline ValArray<Size, T>& ValArray<Size, T>::operator*=(const T& rhs) {
    for (size_t i = 0; i < Size; ++i) {
        _data[i] *= rhs;
    }

    return *this;
}

template <size_t Size, typename T>
inline ValArray<Size, T>& ValArray<Size, T>::operator/=(const T& rhs) {
    for (size_t i = 0; i < Size; ++i) {
        _data[i] /= rhs;
    }

    return *this;
}

// ValArray/ValArray operations
template <size_t Size, typename T>
inline ValArray<Size, T>& ValArray<Size, T>::operator+=(const ValArray<Size, T>& rhs) {
    for (size_t i = 0; i < Size; ++i) {
        _data[i] += rhs[i];
    }

    return *this;
}

template <size_t Size, typename T>
inline ValArray<Size, T>& ValArray<Size, T>::operator-=(const ValArray<Size, T>& rhs) {
    for (size_t i = 0; i < Size; ++i) {
        _data[i] -= rhs[i];
    }

    return *this;
}

template <size_t Size, typename T>
inline ValArray<Size, T>& ValArray<Size, T>::operator*=(const ValArray<Size, T>& rhs) {
    for (size_t i = 0; i < Size; ++i) {
        _data[i] *= rhs[i];
    }

    return *this;
}

template <size_t Size, typename T>
inline ValArray<Size, T>& ValArray<Size, T>::operator/=(const ValArray<Size, T>& rhs) {
    for (size_t i = 0; i < Size; ++i) {
        _data[i] /= rhs[i];
    }

    return *this;
}


// ValArray/Scalar operations
template <size_t Size, typename T>
inline ValArray<Size, T> operator+(const ValArray<Size, T>& lhs, const T& rhs) {
    ValArray<Size, T> array{lhs};

    for (size_t i = 0; i < Size; ++i) {
        array[i] += rhs;
    }

    return array;
}

template <size_t Size, typename T>
inline ValArray<Size, T> operator-(const ValArray<Size, T>& lhs, const T& rhs) {
    ValArray<Size, T> array{lhs};

    for (size_t i = 0; i < Size; ++i) {
        array[i] -= rhs;
    }

    return array;
}

template <size_t Size, typename T>
inline ValArray<Size, T> operator*(const ValArray<Size, T>& lhs, const T& rhs) {
    ValArray<Size, T> array{lhs};

    for (size_t i = 0; i < Size; ++i) {
        array[i] *= rhs;
    }

    return array;
}

template <size_t Size, typename T>
inline ValArray<Size, T> operator/(const ValArray<Size, T>& lhs, const T& rhs) {
    ValArray<Size, T> array{lhs};

    for (size_t i = 0; i < Size; ++i) {
        array[i] /= rhs;
    }

    return array;
}

template <size_t Size, typename T>
inline ValArray<Size, T> operator+(const T& lhs, const ValArray<Size, T>& rhs) {
    ValArray<Size, T> array{rhs};

    for (size_t i = 0; i < Size; ++i) {
        array[i] = lhs + array[i];
    }

    return array;
}

template <size_t Size, typename T>
inline ValArray<Size, T> operator-(const T& lhs, const ValArray<Size, T>& rhs) {
    ValArray<Size, T> array{rhs};

    for (size_t i = 0; i < Size; ++i) {
        array[i] = lhs - array[i];
    }

    return array;
}

template <size_t Size, typename T>
inline ValArray<Size, T> operator*(const T& lhs, const ValArray<Size, T>& rhs) {
    ValArray<Size, T> array{rhs};

    for (size_t i = 0; i < Size; ++i) {
        array[i] = lhs * array[i];
    }

    return array;
}

template <size_t Size, typename T>
inline ValArray<Size, T> operator/(const T& lhs, const ValArray<Size, T>& rhs) {
    ValArray<Size, T> array{rhs};

    for (size_t i = 0; i < Size; ++i) {
        array[i] = lhs / array[i];
    }

    return array;
}

// ValArray/ValArray operations
template <size_t Size, typename T>
inline ValArray<Size, T> operator+(const ValArray<Size, T>& lhs, const ValArray<Size, T>& rhs) {
    ValArray<Size, T> array{lhs};

    for (size_t i = 0; i < Size; ++i) {
        array[i] += rhs[i];
    }

    return array;
}

template <size_t Size, typename T>
inline ValArray<Size, T> operator-(const ValArray<Size, T>& lhs, const ValArray<Size, T>& rhs) {
    ValArray<Size, T> array{lhs};

    for (size_t i = 0; i < Size; ++i) {
        array[i] -= rhs[i];
    }

    return array;
}

template <size_t Size, typename T>
inline ValArray<Size, T> operator*(const ValArray<Size, T>& lhs, const ValArray<Size, T>& rhs) {
    ValArray<Size, T> array{lhs};

    for (size_t i = 0; i < Size; ++i) {
        array[i] *= rhs[i];
    }

    return array;
}

template <size_t Size, typename T>
inline ValArray<Size, T> operator/(const ValArray<Size, T>& lhs, const ValArray<Size, T>& rhs) {
    ValArray<Size, T> array{lhs};

    for (size_t i = 0; i < Size; ++i) {
        array[i] /= rhs[i];
    }

    return array;
}

template <size_t Size, typename T>
inline bool operator==(const ValArray<Size, T>& lhs, const ValArray<Size, T>& rhs) {
    return std::equal(std::begin(lhs.data()), std::end(lhs.data()), std::begin(rhs.data()), std::end(rhs.data()));
}

template <size_t Size, typename T>
inline bool operator!=(const ValArray<Size, T>& lhs, const ValArray<Size, T>& rhs) {
    return !std::equal(std::begin(lhs.data()), std::end(lhs.data()), std::begin(rhs.data()), std::end(rhs.data()));
}
