#include "Quaternion.h"



Quaternion::Quaternion() {}


lug::Maths::Quaternion::Quaternion(double x, double y, double z, double w) : _X(x), _Y(y), _Z(z), _W(w) {

}

lug::Maths::Quaternion::Quaternion(lug::Maths::Vector3D v, double w) : _X(v.getX()), _Y(v.getY()), _Z(v.getZ()), _W(w) {}

lug::Maths::Quaternion::Quaternion(const Quaternion & otherQuaternion) : _X(otherQuaternion.getX()), _Y(otherQuaternion.getY()), _Z(otherQuaternion.getZ()), _W(otherQuaternion.getW()) {}

lug::Maths::Quaternion & lug::Maths::Quaternion::operator=(const lug::Maths::Quaternion & leftOperand) {
    
    _X = leftOperand.getX();
    _Y = leftOperand.getY();
    _Z = leftOperand.getZ();
    _Z = leftOperand.getW();
    return *(this)

}

lug::Maths::Quaternion::~Quaternion() {}

lug::Maths::Quaternion lug::Maths::Quaternion::conjugate() {
    lug::Maths::Quaternion conjugateQuaternion(_X, -_Y, -_Z, -_W);
    return std::move(conjugateQuaternion);
}

lug::Maths::Quaternion lug::Maths::Quaternion::invert() {
    double numerator = std::pow(_X, 2) + std::pow(_Y, 2) + std::pow(_Z, 2) + std::pow(_W, 2);

    lug::Maths::Quaternion inverseQuaternion(_X / numerator, _Y / numerator, _Z / numerator, _W / numerator);
    
    return std::move(inverseQuaternion);
}

lug::Maths::Quaternion lug::Maths::Quaternion::normalize() {
    double numerator = std::sqrt(std::pow(_X, 2) + std::pow(_Y, 2) + std::pow(_Z, 2) + std::pow(_W, 2));

    lug::Maths::Quaternion normalizeQuaternion(_X / numerator, _Y / numerator, _Z / numerator, _W / numerator);

    return std::move(normalize);
}

const double lug::Maths::Quaternion::angleWith(const Quaternion & otherQuaternion) {
    lug::Maths::Quaternion resQuaternion = otherQuaternion * invert();


    return std::move(std::acos(resQuaternion._W) * 2.0);
}

lug::Maths::Quaternion lug::Maths::Quaternion::slerp(const Quaternion &otherQuaternion, double t) {
    
    double angle = angleWith(otherQuaternion) / 2;
    lug::Maths::Quaternion qm = ((*this) * std::sin(1.0 - t) * angle + otherQuaternion * std::sin(t * angle)) / std::sin(angle);
}
