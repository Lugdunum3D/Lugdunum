#include <lug/Math/Quaternion.hpp>


lug::Math::Quaternion::Quaternion(double x, double y, double z, double w) : _x(x), _y(y), _z(z), _w(w) {
    _Axis = lug::Math::Vec3d{y,z,w};
}

lug::Math::Quaternion::Quaternion(lug::Math::Vec3d v) : _Axis(v), _x(0), _y(v.x()), _z(v.y()), _w(v.z()) {}

lug::Math::Quaternion::Quaternion(const Quaternion & otherQuaternion) : _x(otherQuaternion.getX()), _y(otherQuaternion.getY()), _z(otherQuaternion.getZ()), _w(otherQuaternion.getW()) {}

lug::Math::Quaternion & lug::Math::Quaternion::operator=(const lug::Math::Quaternion & leftOperand) {

    _x = leftOperand.getX();
    _y = leftOperand.getY();
    _z = leftOperand.getZ();
    _z = leftOperand.getW();
    return *(this);
}

lug::Math::Quaternion::~Quaternion() {}

lug::Math::Quaternion lug::Math::Quaternion::conjugate() const {
    return lug::Math::Quaternion{_x, -_y, -_z, -_w};
}

lug::Math::Quaternion lug::Math::Quaternion::invert() {
    const double numerator = std::pow(_x, 2) + std::pow(_y, 2) + std::pow(_z, 2) + std::pow(_w, 2);
    return lug::Math::Quaternion{_x / numerator, _y / numerator, _z / numerator, _w / numerator};
}

lug::Math::Quaternion lug::Math::Quaternion::normalize() {
    const double numerator = std::sqrt(std::pow(_x, 2) + std::pow(_y, 2) + std::pow(_z, 2) + std::pow(_w, 2));
    return lug::Math::Quaternion{_x / numerator, _y / numerator, _z / numerator, _w / numerator};
}

double lug::Math::Quaternion::angleWith(const Quaternion & otherQuaternion) {
    lug::Math::Quaternion resQuaternion = otherQuaternion * invert();


    return std::move(std::acos(resQuaternion._w) * 2.0);
}

lug::Math::Quaternion lug::Math::Quaternion::slerp(const Quaternion &otherQuaternion, double t) {

    double angle = angleWith(otherQuaternion) / 2;
    lug::Math::Quaternion qm = ((*this) * std::sin(1.0 - t) * angle + otherQuaternion * std::sin(t * angle)) / std::sin(angle);
    return qm;
}

double lug::Math::Quaternion::getAngle() const {
    return _angle;
}

void lug::Math::Quaternion::setAngle(double angle) {
    _angle = angle;
}

double lug::Math::Quaternion::getX() const {
    return _x;
}

void lug::Math::Quaternion::setX(double X) {
    _x = X;
}

double lug::Math::Quaternion::getY() const {
    return _y;
}

void lug::Math::Quaternion::setY(double Y) {
    _y = Y;
}

double lug::Math::Quaternion::getZ() const {
    return _z;
}

void lug::Math::Quaternion::setZ(double Z) {
    _z = Z;
}

double lug::Math::Quaternion::getW() const {
    return _w;
}

void lug::Math::Quaternion::setW(double W) {
    _w = W;
}

const lug::Math::Vec3d & lug::Math::Quaternion::getAxis() const {
    return _Axis;
}

void lug::Math::Quaternion::setAxis(const lug::Math::Vec3d &axis) {
    _Axis = axis;
    _y = axis.x();
    _z = axis.y();
    _w = axis.z();
}

lug::Math::Quaternion lug::Math::operator+(const lug::Math::Quaternion & leftOperand, const lug::Math::Quaternion & rightOperand) {
    return std::move(lug::Math::Quaternion(leftOperand.getX() + rightOperand.getX(), leftOperand.getY() + rightOperand.getY(), leftOperand.getZ() + rightOperand.getZ(), leftOperand.getW() + rightOperand.getW()));
}

lug::Math::Quaternion lug::Math::operator-(const lug::Math::Quaternion & leftOperand, const lug::Math::Quaternion & rightOperand) {
    return std::move(lug::Math::Quaternion(leftOperand.getX() - rightOperand.getX(), leftOperand.getY() - rightOperand.getY(), leftOperand.getZ() - rightOperand.getZ(), leftOperand.getW() - rightOperand.getW()));
}

lug::Math::Quaternion lug::Math::operator*(const lug::Math::Quaternion & leftOperand, const lug::Math::Quaternion & rightOperand) {
    return std::move(lug::Math::Quaternion(leftOperand.getX() * rightOperand.getX() - leftOperand.getY() * rightOperand.getY() - leftOperand.getZ() * rightOperand.getZ() - leftOperand.getW() * rightOperand.getW(),
                                            leftOperand.getX() * rightOperand.getX() + leftOperand.getY() * rightOperand.getY() + leftOperand.getZ() * rightOperand.getZ() - leftOperand.getW() * rightOperand.getW(),
                                            leftOperand.getX() * rightOperand.getX() - leftOperand.getY() * rightOperand.getY() + leftOperand.getZ() * rightOperand.getZ() + leftOperand.getW() * rightOperand.getW(),
                                            leftOperand.getX() * rightOperand.getX() + leftOperand.getY() * rightOperand.getY() - leftOperand.getZ() * rightOperand.getZ() + leftOperand.getW() * rightOperand.getW()));
}

lug::Math::Quaternion operator/(const lug::Math::Quaternion &leftOperand, const lug::Math::Quaternion &rightOperand) {
    return std::move(leftOperand * rightOperand.conjugate());
}


bool lug::Math::operator==(const lug::Math::Quaternion & leftOperand, const lug::Math::Quaternion & rightOperand) {
    if ((leftOperand.getX() == rightOperand.getX())
        && (leftOperand.getY() == rightOperand.getY())
        && (leftOperand.getZ() == rightOperand.getZ())
        && (leftOperand.getW() == rightOperand.getW())) {
        return true;
    } else { return false; }
}

lug::Math::Quaternion lug::Math::operator*(const lug::Math::Quaternion & leftOperand, double scalar) {
    lug::Math::Quaternion result(leftOperand);
    result.setX(result.getX() * scalar);
    return result;
}

lug::Math::Quaternion lug::Math::operator+(const lug::Math::Quaternion & leftOperand, double scalar) {
    lug::Math::Quaternion result(leftOperand);
    result.setX(result.getX() + scalar);
    return result;
}

lug::Math::Quaternion lug::Math::operator-(const lug::Math::Quaternion & leftOperand, double scalar) {
    lug::Math::Quaternion result(leftOperand);
    result.setX(result.getX() - scalar);
    return result;
}

lug::Math::Quaternion lug::Math::operator/(const lug::Math::Quaternion & leftOperand, double scalar) {
    lug::Math::Quaternion result(leftOperand);
    result.setX(result.getX() / scalar);
    return result;
}

lug::Math::Quaternion lug::Math::reflection(const lug::Math::Quaternion & inputePoint, const Vec3d & rotationAxis) {
    lug::Math::Quaternion in(0, inputePoint.getX(), inputePoint.getY(), inputePoint.getZ());
    lug::Math::Quaternion q(rotationAxis);

    return std::move(q * in * q);
}

lug::Math::Quaternion lug::Math::reflection(const lug::Math::Quaternion & inputePoint, const lug::Math::Quaternion & reflectionPlan) {
    lug::Math::Quaternion in(0, inputePoint.getX(), inputePoint.getY(), inputePoint.getZ());

    lug::Math::Quaternion q(reflectionPlan);
    q.normalize();
    q.setX(0.0);
    return std::move(q * in * q);
}

/*lug::Math::Quaternion lug::Math::rotation(const Quaternion & inputePoint, double angle, const Vec3d & rotationAxis) {
    double halfAngle = angle / 2.0;
    double sinHalfAngle = std::sin(halfAngle);
    lug::Math::Quaternion q(std::cos(halfAngle), sinHalfAngle * rotationAxis.x, sinHalfAngle * rotationAxis.y, sinHalfAngle * rotationAxis.z);

    return std::move(q * inputePoint * q.conjugate());
}*/

lug::Math::Quaternion lug::Math::rotation(const lug::Math::Quaternion & inputePoint, const lug::Math::Quaternion & q) {
    return std::move(q * inputePoint * q.conjugate());
}
