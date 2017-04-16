template <uint8_t Rows, typename T>
inline constexpr Vector<Rows, T>::Vector(T value) : Matrix<Rows, 1, T>(value) {}

template <uint8_t Rows, typename T>
inline Vector<Rows, T>::Vector(std::initializer_list<T> list) : Matrix<Rows, 1, T>(list) {}

template <uint8_t Rows, typename T>
inline Vector<Rows, T>::Vector(const typename Vector<Rows, T>::BaseMatrix& matrix) : Matrix<Rows, 1, T>(matrix) {}

template <uint8_t Rows, typename T>
inline Vector<Rows, T>::Vector(typename Vector<Rows, T>::BaseMatrix&& matrix) : Matrix<Rows, 1, T>(std::move(matrix)) {}

template <uint8_t Rows, typename T>
inline Vector<Rows, T>::Vector(const Vector<Rows - 1, T>& vector, T value) {
    for (uint8_t row = 0; row < Rows - 1; ++row) {
        (*this)(row) = vector(row);
    }

    (*this)(Rows - 1) = value;
}

template <uint8_t Rows, typename T>
inline Vector<Rows, T>::Vector(const Vector<Rows + 1, T>& vector) {
    for (uint8_t row = 0; row < Rows; ++row) {
        (*this)(row) = vector(row);
    }
}

template <uint8_t Rows, typename T>
inline Vector<Rows, T> Vector<Rows, T>::operator*=(const Matrix<Rows, Rows, T>& rhs) {
    Vector<Rows, T> tmp(0);

    for (uint8_t row = 0; row < Rows; ++row) {
        for (uint8_t col = 0; col < Rows; ++col) {
            tmp(row) += (*this)(col) * rhs(row, col);
        }
    }

    *this = std::move(tmp);

    return *this;
}

template <uint8_t Rows, typename T>
inline Vector<Rows, T> Vector<Rows, T>::operator*=(const Vector<Rows, T>& rhs) {
    BaseMatrix::_values *= rhs.getValues();
    return *this;
}

template <uint8_t Rows, typename T>
inline Vector<Rows, T> Vector<Rows, T>::operator/=(const Vector<Rows, T>& rhs) {
    BaseMatrix::_values /= rhs.getValues();
    return *this;
}

template <uint8_t Rows, typename T>
inline constexpr T Vector<Rows, T>::length() const {
    return T(std::sqrt(squaredLength()));
}

template <uint8_t Rows, typename T>
inline constexpr T Vector<Rows, T>::squaredLength() const {
    return T((BaseMatrix::_values * BaseMatrix::_values).sum());
}

template <uint8_t Rows, typename T>
inline void Vector<Rows, T>::normalize() {
    *this = ::lug::Math::normalize(*this);
}

template <typename T>
inline constexpr Vector<3, T> cross(const Vector<3, T>& lhs, const Vector<3, T>& rhs) {
    return {
        lhs.y() * rhs.z() - lhs.z() * rhs.y(),
        lhs.z() * rhs.x() - lhs.x() * rhs.z(),
        lhs.x() * rhs.y() - lhs.y() * rhs.x()
    };
}

template <uint8_t Rows, typename T>
inline constexpr T dot(const Vector<Rows, T>& lhs, const Vector<Rows, T>& rhs) {
    return (lhs.getValues() * rhs.getValues()).sum();
}

template <uint8_t Rows, uint8_t Columns, typename T>
inline constexpr Matrix<Rows, Columns, T> outer(const Vector<Rows, T>& lhs, const Vector<Columns, T>& rhs) {
    return lhs * rhs.transpose();
}

template <uint8_t Rows, typename T>
inline constexpr Vector<Rows, T> normalize(const Vector<Rows, T>& lhs) {
    return lhs / lhs.length();
}

template <uint8_t Rows, typename T>
inline Vector<Rows, T> operator*(const Vector<Rows, T>& lhs, const Vector<Rows, T>& rhs) {
    return {lhs.getValues() * rhs.getValues()};
}

template <uint8_t Rows, typename T>
inline Vector<Rows, T> operator/(const Vector<Rows, T>& lhs, const Vector<Rows, T>& rhs) {
    return {lhs.getValues() / rhs.getValues()};
}

template <uint8_t Rows, typename T>
inline Vector<Rows, T> operator*(const Vector<Rows, T>& lhs, const Matrix<Rows, Rows, T>& rhs) {
    Vector<Rows, T> vector{lhs};

    vector *= rhs;

    return vector;
}

template <uint8_t Rows, typename T>
inline Vector<Rows, T> operator*(const Matrix<Rows, Rows, T>& lhs, const Vector<Rows, T>& rhs) {
    Vector<Rows, T> vector{rhs};

    vector *= lhs;

    return vector;
}

template <typename T>
inline Vector<3, T> operator*(const Vector<3, T>& lhs, const Matrix<4, 4, T>& rhs) {
    return Vector<4, T>{lhs, T(1)} * rhs;
}

template <typename T>
inline Vector<3, T> operator*(const Matrix<4, 4, T>& lhs, const Vector<3, T>& rhs) {
    return lhs * Vector<4, T>{rhs, T(1)};
}
