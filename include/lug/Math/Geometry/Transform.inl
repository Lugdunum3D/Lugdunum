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
