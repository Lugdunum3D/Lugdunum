
template <typename T >
inline lug::Math::Quaternion<T>::Quaternion(T w, T x, T y, T z) {
    _valuesArray[0] = w;
    _valuesArray[1] = x;
    _valuesArray[2] = y;
    _valuesArray[3] = z;
}


template <typename T >
inline lug::Math::Quaternion<T>::Quaternion(T valueArray[4]) {
    _valuesArray[0] = valueArray[0];
    _valuesArray[1] = valueArray[1];
    _valuesArray[2] = valueArray[2];
    _valuesArray[3] = valueArray[3];
}

template <typename T >
inline lug::Math::Quaternion<T>::Quaternion(lug::Math::Vector<3, T> normalizedVector, T degreeAngle) {
    LUG_ASSERT((normalizedVector(0, 0) * normalizedVector(0, 0) + normalizedVector(0, 1) * normalizedVector(0, 1) + normalizedVector(0, 2) * normalizedVector(0, 2)) != 1, "Quaternion construct with not normalized vector");

    T halfAngleRadian = (degreeAngle / 2) * M_PI / 180.0;
    T sinHalfAngle = sin(halfAngleRadian);

    _valuesArray[0] = cos(halfAngleRadian);
    _valuesArray[1] = normalizedVector(0, 0) * sinHalfAngle;
    _valuesArray[2] = normalizedVector(0, 1) * sinHalfAngle;
    _valuesArray[3] = normalizedVector(0, 2)* sinHalfAngle;
}


template <typename T >
inline lug::Math::Quaternion<T>::Quaternion(const lug::Math::Quaternion<T> & otherQuaternion) {
    *(this) = otherQuaternion;
}


template <typename T >
inline  lug::Math::Quaternion<T>& lug::Math::Quaternion<T>::operator=(const  lug::Math::Quaternion<T>& leftOperand) {

    _valuesArray[0] = leftOperand[0];
    _valuesArray[1] = leftOperand[1];
    _valuesArray[2] = leftOperand[2];
    _valuesArray[3] = leftOperand[3];
    return *(this);
}


template <typename T >
inline lug::Math::Quaternion<T>::~Quaternion() {}


template <typename T >
inline T & lug::Math::Quaternion<T>::operator[](std::size_t idx) {
    return _valuesArray[idx];
}


template <typename T >
inline const T & lug::Math::Quaternion<T>::operator[](std::size_t idx) const {
    return _valuesArray[idx];
}

// lug::Math::Quaternion<T>lug::Math::Quaternion::conjugate() const {
//    return lug::Math::Quaternion{_x, -_y, -_z, -_w};
//}

// lug::Math::Quaternion<T>lug::Math::Quaternion::invert() {
//    const T numerator = std::pow(_x, 2) + std::pow(_y, 2) + std::pow(_z, 2) + std::pow(_w, 2);
//    return lug::Math::Quaternion{_x / numerator, _y / numerator, _z / numerator, _w / numerator};
//}

// lug::Math::Quaternion<T>lug::Math::Quaternion::normalize() {
//    const T numerator = std::sqrt(std::pow(_x, 2) + std::pow(_y, 2) + std::pow(_z, 2) + std::pow(_w, 2));
//    return lug::Math::Quaternion{_x / numerator, _y / numerator, _z / numerator, _w / numerator};
//}
//
//T lug::Math::Quaternion::angleWith(const lug::Math::Quaternion<T> & otherQuaternion) {
//     lug::Math::Quaternion<T>resQuaternion = otherQuaternion * invert();
//
//
//    return std::move(std::acos(resQuaternion._w) * 2.0);
//}
//
// lug::Math::Quaternion<T>lug::Math::Quaternion::slerp(const lug::Math::Quaternion<T> &otherQuaternion, T t) {
//
//    T angle = angleWith(otherQuaternion) / 2;
//     lug::Math::Quaternion<T>qm = ((*this) * std::sin(1.0 - t) * angle + otherQuaternion * std::sin(t * angle)) / std::sin(angle);
//    return qm;
//}
//
//T lug::Math::Quaternion::getAngle() const {
//    return _angle;
//}
//
//void lug::Math::Quaternion::setAngle(T angle) {
//    _angle = angle;
//}
//


template <typename T >
inline  lug::Math::Quaternion<T> lug::Math::operator+(const  lug::Math::Quaternion<T>& leftOperand, const  lug::Math::Quaternion<T>& rightOperand) {
    return std::move(lug::Math::Quaternion(leftOperand[0] + rightOperand[0], leftOperand[1] + rightOperand[1], leftOperand[2] + rightOperand[2], leftOperand[3] + rightOperand[3]));
}

template <typename T >
inline  lug::Math::Quaternion<T> lug::Math::operator-(const  lug::Math::Quaternion<T>& leftOperand, const  lug::Math::Quaternion<T>& rightOperand) {
    return std::move(lug::Math::Quaternion(leftOperand[0] - rightOperand[0], leftOperand[1] - rightOperand[1], leftOperand[2] - rightOperand[2], leftOperand[3] - rightOperand[3]));
}

template <typename T >
inline  lug::Math::Quaternion<T> lug::Math::operator*(const  lug::Math::Quaternion<T>& leftOperand, const  lug::Math::Quaternion<T>& rightOperand) {
    return std::move(lug::Math::Quaternion(leftOperand[0] * rightOperand[0] - leftOperand[1] * rightOperand[1] - leftOperand[2] * rightOperand[2] - leftOperand[3] * rightOperand[3],
        leftOperand[0] * rightOperand[0] + leftOperand[1] * rightOperand[1] + leftOperand[2] * rightOperand[2] - leftOperand[3] * rightOperand[3],
        leftOperand[0] * rightOperand[0] - leftOperand[1] * rightOperand[1] + leftOperand[2] * rightOperand[2] + leftOperand[3] * rightOperand[3],
        leftOperand[0] * rightOperand[0] + leftOperand[1] * rightOperand[1] - leftOperand[2] * rightOperand[2] + leftOperand[3] * rightOperand[3]));
}

// lug::Math::Quaternion<T>operator/(const  lug::Math::Quaternion<T>&leftOperand, const  lug::Math::Quaternion<T>&rightOperand) {
//    return std::move(leftOperand * rightOperand.conjugate());
//}


template <typename T >
inline bool lug::Math::operator==(const  lug::Math::Quaternion<T>& leftOperand, const  lug::Math::Quaternion<T>& rightOperand) {
    if ((leftOperand[0] == rightOperand[0])
        && (leftOperand[1] == rightOperand[1])
        && (leftOperand[2] == rightOperand[2])
        && (leftOperand[3] == rightOperand[3])) {
        return true;
    }
    else { return false; }
}

template <typename T >
inline  lug::Math::Quaternion<T> lug::Math::operator*(const  lug::Math::Quaternion<T>& leftOperand, T scalar) {
    lug::Math::Quaternion<T>result(leftOperand);
    result[0] = (result[0] * scalar);
    return result;
}

template <typename T >
inline  lug::Math::Quaternion<T>lug::Math::operator+(const  lug::Math::Quaternion<T>& leftOperand, T scalar) {
    lug::Math::Quaternion<T>result(leftOperand);
    result[0] = (result[0] + scalar);
    return result;
}

template <typename T >
inline  lug::Math::Quaternion<T> lug::Math::operator-(const  lug::Math::Quaternion<T>& leftOperand, T scalar) {
    lug::Math::Quaternion<T>result(leftOperand);
    result[0] = (result[0] - scalar);
    return result;
}

template <typename T >
inline  lug::Math::Quaternion<T> lug::Math::operator/(const  lug::Math::Quaternion<T>& leftOperand, T scalar) {
    lug::Math::Quaternion<T>result(leftOperand);
    result[0] = (result[0] / scalar);
    return result;
}

// lug::Math::Quaternion<T>lug::Math::reflection(const  lug::Math::Quaternion<T>& inputePoint, const Vec3d & rotationAxis) {
//     lug::Math::Quaternion<T>in(0, inputePoint.w, inputePoint.x, inputePoint.y);
//     lug::Math::Quaternion<T>q(rotationAxis);
//
//    return std::move(q * in * q);
//}
//
// lug::Math::Quaternion<T>lug::Math::reflection(const  lug::Math::Quaternion<T>& inputePoint, const  lug::Math::Quaternion<T>& reflectionPlan) {
//     lug::Math::Quaternion<T>in(0, inputePoint.w, inputePoint.x, inputePoint.y);
//
//     lug::Math::Quaternion<T>q(reflectionPlan);
//    q.normalize();
//    q.setX(0.0);
//    return std::move(q * in * q);
//}

/* lug::Math::Quaternion<T>lug::Math::rotation(const lug::Math::Quaternion<T> & inputePoint, T angle, const Vec3d & rotationAxis) {
T halfAngle = angle / 2.0;
T sinHalfAngle = std::sin(halfAngle);
lug::Math::Quaternion<T>q(std::cos(halfAngle), sinHalfAngle * rotationAxis.w, sinHalfAngle * rotationAxis.x, sinHalfAngle * rotationAxis.y);

return std::move(q * inputePoint * q.conjugate());
}*/
//
// lug::Math::Quaternion<T>lug::Math::rotation(const  lug::Math::Quaternion<T>& inputePoint, const  lug::Math::Quaternion<T>& q) {
//    return std::move(q * inputePoint * q.conjugate());
//}
