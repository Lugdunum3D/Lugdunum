template <uint8_t Rows, typename T>
inline constexpr Vector<Rows, T>::Vector(T value) : Matrix<Rows, 1, T>(value) {}

template <uint8_t Rows, typename T>
inline Vector<Rows, T>::Vector(std::initializer_list<T> list) : Matrix<Rows, 1, T>(list) {}

template <uint8_t Rows, typename T>
inline Vector<Rows, T>::Vector(const BaseMatrix& matrix) : Matrix<Rows, 1, T>(matrix) {}

template <uint8_t Rows, typename T>
inline Vector<Rows, T>::Vector(BaseMatrix&& matrix) : Matrix<Rows, 1, T>(std::move(matrix)) {}

template <uint8_t Rows, typename T>
inline Vector<Rows, T>::Vector(const Vector<Rows - 1, T>& vector, T value) {
    for (uint8_t row = 0; row < Rows; ++row) {
        (*this)(row) = vector(row);
    }

    (*this)(Rows - 1) = value;
}

template <uint8_t Rows, typename T>
template <bool EnableBool>
inline constexpr typename std::enable_if<Rows == 3 && EnableBool, Vector<Rows, T>&>::type Vector<Rows, T>::operator^=(const Vector<Rows, T>& rhs) {
    *this = *this ^ rhs;
    return *this;
}

template <uint8_t Rows, typename T>
inline constexpr T Vector<Rows, T>::norme() const {
    return std::sqrt((BaseMatrix::_values * BaseMatrix::_values).sum());
}

template <uint8_t Rows, typename T>
inline constexpr void Vector<Rows, T>::normalize() {
    *this /= norme();
}

template <typename T>
inline constexpr Vector<3, T> operator^(const Vector<3, T>& lhs, const Vector<3, T>& rhs) {
    return Vector<3, T>(
        lhs.y() * rhs.z() - lhs.z() * rhs.y(),
        lhs.z() * rhs.x() - lhs.x() * rhs.z(),
        lhs.x() * rhs.y() - lhs.y() * rhs.x()
    );
}
