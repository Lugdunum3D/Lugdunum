template <typename T>
Quaternion<T>::Quaternion(T w, T x, T y, T z) : _data{w, x, y, z} {}

template <typename T>
Quaternion<T>::Quaternion(T data[4]) : _data{data} {}

template <typename T>
Quaternion<T>::Quaternion(T angle, const Vector<3, T>& axis) {
    const T halfAngle = angle / 2;
    const T sinHalf = std::sin(halfAngle);

    _data[0] = std::cos(halfAngle);
    _data[1] = axis(0) * sinHalf;
    _data[2] = axis(1) * sinHalf;
    _data[3] = axis(2) * sinHalf;
}

template <typename T>
inline T& Quaternion<T>::operator[](std::size_t idx) {
    return _data[idx];
}

template <typename T>
inline const T& Quaternion<T>::operator[](std::size_t idx) const {
    return _data[idx];
}

template <typename T>
inline void Quaternion<T>::conjugate() {
    *this = ::lug::Math::conjugate(*this);
}

template <typename T>
inline void Quaternion<T>::inverse() {
    *this = ::lug::Math::conjugate(*this);
}

template <typename T>
inline void Quaternion<T>::normalize() {
    *this = ::lug::Math::normalize(*this);
}

template <typename T>
inline T Quaternion<T>::length() const {
    return std::sqrt(_data[0] * _data[0] + _data[1] * _data[1] + _data[2] * _data[2] + _data[3] * _data[3]);
}

template <typename T>
inline T Quaternion<T>::getAngle() const {
    return std::acos(_data[0]) * 2;
}

template <typename T>
inline Vector<3, T> Quaternion<T>::getAxis() const {
    const T asinHalf = std::asin(getAngle() / 2);
    return {
        _data[1] / asinHalf,
        _data[2] / asinHalf,
        _data[3] / asinHalf
    };
}

template <typename T>
inline Quaternion<T> conjugate(const Quaternion<T>& lhs) {
    return {lhs.w(), -lhs.x(), -lhs.y(), -lhs.z()};
}

template <typename T>
inline Quaternion<T> normalize(const Quaternion<T>& lhs) {
    const T length = lhs.length();
    return {lhs[0] / length, lhs[1] / length, lhs[2] / length, lhs[3] / length};
}

template <typename T>
inline Quaternion<T> inverse(const Quaternion<T>& lhs) {
    Quaternion<T> result{lhs};

    result.conjugate();

    const T length = result.length();

    result[0] /= length * length;
    result[1] /= length * length;
    result[2] /= length * length;
    result[3] /= length * length;

    return result;
}

template <typename T>
inline T dot(const Quaternion<T>& lhs, const Quaternion<T>& rhs) {
    return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2] + lhs[3] * rhs[3];
}

template <typename T>
inline Quaternion<T> operator-(const Quaternion<T>& lhs) {
    return {-lhs[0], -lhs[1], -lhs[2], -lhs[3]};
}

template <typename T>
inline Quaternion<T> operator+(const Quaternion<T>& lhs, const Quaternion<T>& rhs) {
    return {lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2], lhs[3] + rhs[3]};
}

template <typename T>
inline Quaternion<T> operator-(const Quaternion<T>& lhs, const Quaternion<T>& rhs) {
    return {lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2], lhs[3] - rhs[3]};
}

template <typename T>
inline Quaternion<T> operator*(const Quaternion<T>& lhs, const Quaternion<T>& rhs) {
    return {
        lhs[0] * rhs[0] - lhs[1] * rhs[1] - lhs[2] * rhs[2] - lhs[3] * rhs[3],
        lhs[0] * rhs[1] + lhs[1] * rhs[0] + lhs[2] * rhs[3] - lhs[3] * rhs[2],
        lhs[0] * rhs[2] - lhs[1] * rhs[3] + lhs[2] * rhs[0] + lhs[3] * rhs[1],
        lhs[0] * rhs[3] + lhs[1] * rhs[2] - lhs[2] * rhs[1] + lhs[3] * rhs[0]
    };
}

template <typename T>
inline Quaternion<T> operator/(const Quaternion<T>& lhs, const Quaternion<T>& rhs) {
    return lhs * conjugate(rhs);
}

template <typename T>
inline bool operator==(const Quaternion<T>& lhs, const Quaternion<T>& rhs) {
    return lhs[0] == rhs[0] && lhs[1] == rhs[1] && lhs[2] == rhs[2] && lhs[3] == rhs[3];
}

template <typename T>
inline bool operator!=(const Quaternion<T>& lhs, const Quaternion<T>& rhs) {
    return lhs[0] != rhs[0] || lhs[1] != rhs[1] || lhs[2] != rhs[2] || lhs[3] != rhs[3];
}
