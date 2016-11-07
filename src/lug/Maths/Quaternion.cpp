#include <lug\Maths\Quaternion.hpp>


lug::Maths::Quaternion::Quaternion(double x, double y, double z, double w) : _x(x), _y(y), _z(z), _w(w) {
    _Axis = lug::Maths::Vec3d{y,z,w};
}

lug::Maths::Quaternion::Quaternion(lug::Maths::Vec3d v) : _x(0), _y(v.x), _z(v.y), _w(v.z), _Axis(v) {}

lug::Maths::Quaternion::Quaternion(const Quaternion & otherQuaternion) : _x(otherQuaternion.getX()), _y(otherQuaternion.getY()), _z(otherQuaternion.getZ()), _w(otherQuaternion.getW()) {}

lug::Maths::Quaternion & lug::Maths::Quaternion::operator=(const lug::Maths::Quaternion & leftOperand) {
    
    _x = leftOperand.getX();
    _y = leftOperand.getY();
    _z = leftOperand.getZ();
    _z = leftOperand.getW();
    return *(this);
}

lug::Maths::Quaternion::~Quaternion() {}

lug::Maths::Quaternion lug::Maths::Quaternion::conjugate() const {
    lug::Maths::Quaternion conjugateQuaternion(_x, -_y, -_z, -_w);
    return std::move(conjugateQuaternion);
}

lug::Maths::Quaternion lug::Maths::Quaternion::invert() {
    double numerator = std::pow(_x, 2) + std::pow(_y, 2) + std::pow(_z, 2) + std::pow(_w, 2);

    lug::Maths::Quaternion inverseQuaternion(_x / numerator, _y / numerator, _z / numerator, _w / numerator);
    
    return std::move(inverseQuaternion);
}

lug::Maths::Quaternion lug::Maths::Quaternion::normalize() {
    double numerator = std::sqrt(std::pow(_x, 2) + std::pow(_y, 2) + std::pow(_z, 2) + std::pow(_w, 2));

    lug::Maths::Quaternion normalizeQuaternion(_x / numerator, _y / numerator, _z / numerator, _w / numerator);

    return std::move(normalizeQuaternion);
}

const double lug::Maths::Quaternion::angleWith(const Quaternion & otherQuaternion) {
    lug::Maths::Quaternion resQuaternion = otherQuaternion * invert();


    return std::move(std::acos(resQuaternion._w) * 2.0);
}

lug::Maths::Quaternion lug::Maths::Quaternion::slerp(const Quaternion &otherQuaternion, double t) {
    
    double angle = angleWith(otherQuaternion) / 2;
    lug::Maths::Quaternion qm = ((*this) * std::sin(1.0 - t) * angle + otherQuaternion * std::sin(t * angle)) / std::sin(angle);
}

const double & lug::Maths::Quaternion::getAngle() const {
    return _angle;
}

void lug::Maths::Quaternion::setAngle(const double &angle) {
    _angle = angle;
}

const double & lug::Maths::Quaternion::getX() const {
    return _x;
}

void lug::Maths::Quaternion::setX(const double & X) {
    _x = X;
}

const double & lug::Maths::Quaternion::getY() const {
    return _y;
}

void lug::Maths::Quaternion::setY(const double & Y) {
    _y = Y;
}

const double & lug::Maths::Quaternion::getZ() const {
    return _z;
}

void lug::Maths::Quaternion::setZ(const double & Z) {
    _z = Z;
}

const double & lug::Maths::Quaternion::getW() const {
    return _w;
}

void lug::Maths::Quaternion::setW(const double &W) {
    _w = W;
}

const lug::Maths::Vec3d & lug::Maths::Quaternion::getAxis() const {
    return _Axis;
}

void lug::Maths::Quaternion::setAxis(const lug::Maths::Vec3d &axis) {
    _Axis = axis;
    _y = axis.x;
    _z = axis.y;
    _w = axis.z;
}

lug::Maths::Quaternion lug::Maths::operator+(const lug::Maths::Quaternion & leftOperand, const lug::Maths::Quaternion & rightOperand) {
    return std::move(lug::Maths::Quaternion(leftOperand.getX() + rightOperand.getX(), leftOperand.getY() + rightOperand.getY(), leftOperand.getZ() + rightOperand.getZ(), leftOperand.getW() + rightOperand.getW()));
}

lug::Maths::Quaternion lug::Maths::operator-(const lug::Maths::Quaternion & leftOperand, const lug::Maths::Quaternion & rightOperand) {
    return std::move(lug::Maths::Quaternion(leftOperand.getX() - rightOperand.getX(), leftOperand.getY() - rightOperand.getY(), leftOperand.getZ() - rightOperand.getZ(), leftOperand.getW() - rightOperand.getW()));
}

lug::Maths::Quaternion lug::Maths::operator*(const lug::Maths::Quaternion & leftOperand, const lug::Maths::Quaternion & rightOperand) {
    return std::move(lug::Maths::Quaternion(leftOperand.getX() * rightOperand.getX() - leftOperand.getY() * rightOperand.getY() - leftOperand.getZ() * rightOperand.getZ() - leftOperand.getW() * rightOperand.getW(),
                                            leftOperand.getX() * rightOperand.getX() + leftOperand.getY() * rightOperand.getY() + leftOperand.getZ() * rightOperand.getZ() - leftOperand.getW() * rightOperand.getW(),
                                            leftOperand.getX() * rightOperand.getX() - leftOperand.getY() * rightOperand.getY() + leftOperand.getZ() * rightOperand.getZ() + leftOperand.getW() * rightOperand.getW(),
                                            leftOperand.getX() * rightOperand.getX() + leftOperand.getY() * rightOperand.getY() - leftOperand.getZ() * rightOperand.getZ() + leftOperand.getW() * rightOperand.getW()));
}

lug::Maths::Quaternion operator/(const lug::Maths::Quaternion &leftOperand, const lug::Maths::Quaternion &rightOperand) {
    return std::move(leftOperand * rightOperand.conjugate);
}


const bool lug::Maths::operator==(const lug::Maths::Quaternion & leftOperand, const lug::Maths::Quaternion & rightOperand) {
    if ((leftOperand.getX() == rightOperand.getX()) 
        && (leftOperand.getY() == rightOperand.getY()) 
        && (leftOperand.getZ() == rightOperand.getZ()) 
        && (leftOperand.getW() == rightOperand.getW())) {
        return true;
    } else { return false; }
}

lug::Maths::Quaternion lug::Maths::operator*(const lug::Maths::Quaternion & leftOperand, const double scalar) {
    lug::Maths::Quaternion result(leftOperand);
    result.setX(result.getX() * scalar);
    return (std::move(result));
}

lug::Maths::Quaternion lug::Maths::operator+(const lug::Maths::Quaternion & leftOperand, const double scalar) {
    lug::Maths::Quaternion result(leftOperand);
    result.setX(result.getX() + scalar);
    return (std::move(result));
}

lug::Maths::Quaternion lug::Maths::operator-(const lug::Maths::Quaternion & leftOperand, const double scalar) {
    lug::Maths::Quaternion result(leftOperand);
    result.setX(result.getX() - scalar);
    return (std::move(result));
}

lug::Maths::Quaternion lug::Maths::operator/(const lug::Maths::Quaternion & leftOperand, const double scalar) {
    lug::Maths::Quaternion result(leftOperand);
    result.setX(result.getX() / scalar);
    return (std::move(result));
}

lug::Maths::Quaternion lug::Maths::reflection(const lug::Maths::Quaternion & inputePoint, const Vec3d & rotationAxis) {
    lug::Maths::Quaternion in(0, inputePoint.getX(), inputePoint.getY(), inputePoint.getZ());
    lug::Maths::Quaternion q(rotationAxis);

    return std::move(q * in * q);
}

lug::Maths::Quaternion lug::Maths::reflection(const lug::Maths::Quaternion & inputePoint, const lug::Maths::Quaternion & reflectionPlan) {
    lug::Maths::Quaternion in(0, inputePoint.getX(), inputePoint.getY(), inputePoint.getZ());
    
    lug::Maths::Quaternion q(reflectionPlan);
    q.normalize();
    q.setX(0.0);
    return std::move(q * in * q);
}

/*lug::Maths::Quaternion lug::Maths::rotation(const Quaternion & inputePoint, const double & angle, const Vec3d & rotationAxis) {
    double halfAngle = angle / 2.0;
    double sinHalfAngle = std::sin(halfAngle);
    lug::Maths::Quaternion q(std::cos(halfAngle), sinHalfAngle * rotationAxis.x, sinHalfAngle * rotationAxis.y, sinHalfAngle * rotationAxis.z);
    
    return std::move(q * inputePoint * q.conjugate());
}*/

lug::Maths::Quaternion lug::Maths::rotation(const lug::Maths::Quaternion & inputePoint, const lug::Maths::Quaternion & q) {
    return std::move(q * inputePoint * q.conjugate());
}
