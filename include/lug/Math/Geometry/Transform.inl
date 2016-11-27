template <typename T>
inline Matrix<4, 4, T> translate(const Vector<3, T> &direction) {
    Matrix<4, 4, T> matrix = Matrix<4, 4, T>::identity();

    matrix(0, 3) = direction(0);
    matrix(1, 3) = direction(1);
    matrix(2, 3) = direction(2);

    return matrix;
}

template <typename T>
inline Matrix<4, 4, T> rotate(T angle, const Vector<3, T> &a) {
    T const c = ::lug::Math::Geometry::cos(angle);
    T const s = ::lug::Math::Geometry::sin(angle);

    Vector<3, T> axis(normalize(a));
    Vector<3, T> tmp((T(1) - c) * axis);

    return Matrix<4, 4, T> {
        c + tmp.x() * axis.x(),
        tmp.x() * axis.y() + s * axis.z(),
        tmp.x() * axis.z() - s * axis.y(),
        0,

        tmp.y() * axis.x() - s * axis.z(),
        c + tmp.y() * axis.y(),
        tmp.y() * axis.z() + s * axis.x(),
        0,

        tmp.z() * axis.x() + s * axis.y(),
        tmp.z() * axis.y() - s * axis.x(),
        c + tmp.z() * axis.z(),
        0,

        0, 0, 0, 1
    };
}

template <typename T>
inline Matrix<4, 4, T> scale(const Vector<3, T> &factors) {
    Matrix<4, 4, T> matrix = Matrix<4, 4, T>::identity();

    matrix(0, 0) = factors(0);
    matrix(1, 1) = factors(1);
    matrix(2, 2) = factors(2);

    return matrix;
}

template <typename T>
inline Matrix<4, 4, T> lookAt(const Vector<3, T> &eye, const Vector<3, T> &center, const Vector<3, T> &up) {
    const Vector<3, T> direction(normalize(static_cast<Vector<3, T>>(eye - center)));
    const Vector<3, T> right(normalize(cross(up, direction)));
    const Vector<3, T> newUp(cross(direction, right));

    return Matrix<4, 4, T> {
        right.x(),
        right.y(),
        right.z(),
        -dot(right, eye),

        newUp.x(),
        newUp.y(),
        newUp.z(),
        -dot(newUp, eye),

        direction.x(),
        direction.y(),
        direction.z(),
        -dot(direction, eye),

        0,
        0,
        0,
        1,
    };
}

template <typename T>
inline Matrix<4, 4, T> ortho(T left, T right, T bottom, T top, T zNear, T zFar) {
    Matrix<4, 4, T> matrix = Matrix<4, 4, T>::identity();

    matrix(0, 0) = T(2) / (right - left);
    matrix(0, 3) = -(right + left) / (right - left);
    matrix(1, 1) = T(2) / (top - bottom);
    matrix(1, 3) = -(top + bottom) / (top - bottom);
    matrix(2, 2) = -T(1) / (zFar - zNear);
    matrix(2, 3) = -zNear / (zFar - zNear);

    return matrix;
}

template <typename T>
inline Matrix<4, 4, T> perspective (T fovy, T aspect, T zNear, T zFar) {
    const T tanHalfFovy = ::lug::Math::Geometry::tan(fovy / T(2));

    Matrix<4, 4, T> matrix(0);

    matrix(0, 0) = T(1) / (aspect * tanHalfFovy);
    matrix(1, 1) = T(1) / (tanHalfFovy);
    matrix(2, 2) = zFar / (zNear - zFar);
    matrix(2, 3) = -(zFar * zNear) / (zFar - zNear);
    matrix(3, 2) = -T(1);

    return matrix;
}
