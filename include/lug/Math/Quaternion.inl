template <typename T>
Quaternion<T>::Quaternion(T w, T x, T y, T z) : _data{w, x, y, z} {}

template <typename T>
Quaternion<T>::Quaternion(T data[4]) {
    _data[0] = data[0];
    _data[1] = data[1];
    _data[2] = data[2];
    _data[3] = data[3];

    normalize();
}

template <typename T>
Quaternion<T>::Quaternion(T angle, const Vector<3, T>& axis) {
    const T halfAngle = angle / 2;
    const T sinHalf = std::sin(halfAngle);

    _data[0] = std::cos(halfAngle);
    _data[1] = axis(0) * sinHalf;
    _data[2] = axis(1) * sinHalf;
    _data[3] = axis(2) * sinHalf;

    normalize();
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
inline constexpr T Quaternion<T>::length() const {
    return std::sqrt(squaredLength());
}

template <typename T>
inline constexpr T Quaternion<T>::squaredLength() const {
    return _data[0] * _data[0] + _data[1] * _data[1] + _data[2] * _data[2] + _data[3] * _data[3];
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
inline Mat4x4<T> Quaternion<T>::transform() const {
    Mat4x4<T> result{Mat4x4<T>::identity()};

    const T xx = _data[1] * _data[1];
    const T xy = _data[1] * _data[2];
    const T xz = _data[1] * _data[3];
    const T wx = _data[0] * _data[1];

    const T yy = _data[2] * _data[2];
    const T yz = _data[2] * _data[3];
    const T wy = _data[0] * _data[2];

    const T zz = _data[3] * _data[3];
    const T wz = _data[0] * _data[3];

    result(0, 0) = T(1) - T(2) * (yy + zz);
    result(1, 0) = T(2) * (xy + wz);
    result(2, 0) = T(2) * (xz - wy);

    result(0, 1) = T(2) * (xy - wz);
    result(1, 1) = T(1) - T(2) * (xx + zz);
    result(2, 1) = T(2) * (yz + wx);

    result(0, 2) = T(2) * (xz + wy);
    result(1, 2) = T(2) * (yz - wx);
    result(2, 2) = T(1) - T(2) * (xx + yy);

    return result;
}

template <typename T>
inline Quaternion<T> Quaternion<T>::identity() {
    return {T(1), T(0), T(0), T(0)};
}

template <typename T>
inline Quaternion<T> Quaternion<T>::fromAxes(const Vector<3, T>& xAxis, const Vector<3, T>& yAxis, const Vector<3, T>& zAxis) {
    Matrix<4, 4, T> rotMatrix = Matrix<4, 4, T>::identity();

    rotMatrix(0, 0) = xAxis.x();
    rotMatrix(1, 0) = xAxis.y();
    rotMatrix(2, 0) = xAxis.z();

    rotMatrix(0, 1) = yAxis.x();
    rotMatrix(1, 1) = yAxis.y();
    rotMatrix(2, 1) = yAxis.z();

    rotMatrix(0, 2) = zAxis.x();
    rotMatrix(1, 2) = zAxis.y();
    rotMatrix(2, 2) = zAxis.z();

    return fromRotationMatrix(rotMatrix);
}

template <typename T>
inline Quaternion<T> Quaternion<T>::fromRotationMatrix(const Matrix<4, 4, T>& rotMatrix) {
    const T tr = rotMatrix(0, 0) + rotMatrix(1, 1) + rotMatrix(2, 2);

    if (tr > 0.0f) {
        const T s = sqrt(T(1) + tr) * T(2);

        return Quaternion<T>(
            T(0.25f) * s,
            (rotMatrix(2, 1) - rotMatrix(1, 2)) / s,
            (rotMatrix(0, 2) - rotMatrix(2, 0)) / s,
            (rotMatrix(1, 0) - rotMatrix(0, 1)) / s
        );
    } else if ((rotMatrix(0, 0) > rotMatrix(1, 1)) && (rotMatrix(0, 0) > rotMatrix(2, 2))) {
        const T s = sqrt(T(1) + rotMatrix(0, 0) - rotMatrix(1, 1) - rotMatrix(2, 2)) * T(2);

        return Quaternion<T>(
            (rotMatrix(2, 1) - rotMatrix(1, 2)) / s,
            T(0.25) * s,
            (rotMatrix(0, 1) + rotMatrix(1, 0)) / s,
            (rotMatrix(0, 2) + rotMatrix(2, 0)) / s
        );
    } else if (rotMatrix(1, 1) > rotMatrix(2, 2)) {
        const T s = sqrt(T(1) + rotMatrix(1, 1) - rotMatrix(0, 0) - rotMatrix(2, 2)) * T(2);

        return Quaternion<T>(
            (rotMatrix(0, 2) - rotMatrix(2, 0)) / s,
            (rotMatrix(0, 1) + rotMatrix(1, 0)) / s,
            T(0.25) * s,
            (rotMatrix(1, 2) + rotMatrix(2, 1)) / s
        );
    } else {
        const T s = sqrt(T(1) + rotMatrix(2, 2) - rotMatrix(0, 0) - rotMatrix(1, 1)) * T(2);

        return Quaternion<T>(
            (rotMatrix(1, 0) - rotMatrix(0, 1)) / s,
            (rotMatrix(0, 2) + rotMatrix(2, 0)) / s,
            (rotMatrix(1, 2) + rotMatrix(2, 1)) / s,
            T(0.25) * s
        );
    }
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
    result[1] /= -length * length;
    result[2] /= -length * length;
    result[3] /= -length * length;

    result.normalize();

    return result;
}

template <typename T>
inline T dot(const Quaternion<T>& lhs, const Quaternion<T>& rhs) {
    return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2] + lhs[3] * rhs[3];
}

template <typename T>
Quaternion<T> directionTo(const Vector<3, T>& original, const Vector<3, T>& expected) {
    Quaternion<T> result;

    Vector<3, T> v0 = normalize(original);
    Vector<3, T> v1 = normalize(expected);

    T cosTheta = dot(v0, v1);

    if (cosTheta >= T(1) - epsilon<T>()) {
        // Same vectors
        return Quaternion<T>::identity();
    }

    if (cosTheta < T(-1) + epsilon<T>()) {
        // Generate a fallback vector
        Vector<3, T> axis = cross(Vector<3, T>{0.0f, 0.0f, 1.0f}, v0);
        if (axis.squaredLength() <= (1e-06 * 1e-06)) {
            axis = cross(Vector<3, T>{1.0f, 0.0f, 0.0f}, v0);
        }

        axis.normalize();
        result = Quaternion<T>(pi<T>(), axis);
    } else {
        T s = sqrt((T(1) + cosTheta) * T(2));
        T invs = T(1) / s;

        Vector<3, T> c = cross(v0, v1);

        result.w() = s * 0.5f;
        result.x() = c.x() * invs;
        result.y() = c.y() * invs;
        result.z() = c.z() * invs;

        result.normalize();
    }

    return result;
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

template <typename T>
std::ostream& operator<<(std::ostream& os, const Quaternion<T>& quaternion) {
    os << "{w: " << quaternion.w() << ", x: " << quaternion.x() << ", y: " << quaternion.y() << ", z: " << quaternion.z() << "}";
    return os;
}
