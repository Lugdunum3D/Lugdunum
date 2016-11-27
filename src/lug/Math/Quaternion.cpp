#include <lug/Math/Quaternion.hpp>


lug::Math::Quaternion::Quaternion(double w, double x, double y, double z){
    _valuesArray[0] = w;
    _valuesArray[1] = x;
    _valuesArray[2] = y;
    _valuesArray[3] = z;
}

lug::Math::Quaternion::Quaternion(double valueArray[4]) {
    _valuesArray[0] = valueArray[0];
    _valuesArray[1] = valueArray[1];
    _valuesArray[2] = valueArray[2];
    _valuesArray[3] = valueArray[3];
}
lug::Math::Quaternion::Quaternion(Vec3d normalizedVector, double degreeAngle){
    LUG_ASSERT((normalizedVector(0,0) * normalizedVector(0, 0) + normalizedVector(0, 1) * normalizedVector(0, 1) + normalizedVector(0, 2) * normalizedVector(0, 2)) != 1, "Quaternion construct with not normalized vector");
    
    double halfAngleRadian = (degreeAngle / 2) * M_PI / 180.0;
    double sinHalfAngle = sin(halfAngleRadian);
  
    _valuesArray[0] = cos(halfAngleRadian);
    _valuesArray[1] = normalizedVector(0, 0) * sinHalfAngle;
    _valuesArray[2] = normalizedVector(0, 1) * sinHalfAngle;
    _valuesArray[3] = normalizedVector(0, 2)* sinHalfAngle;
}

lug::Math::Quaternion::Quaternion(const Quaternion & otherQuaternion){
    *(this) = otherQuaternion;
}

lug::Math::Quaternion & lug::Math::Quaternion::operator=(const lug::Math::Quaternion & leftOperand) {

    _valuesArray[0] = leftOperand[0];
    _valuesArray[1] = leftOperand[1];
    _valuesArray[2] = leftOperand[2];
    _valuesArray[3] = leftOperand[3];
    return *(this);
}

lug::Math::Quaternion::~Quaternion() {}

double & lug::Math::Quaternion::operator[](std::size_t idx) {
    return _valuesArray[idx];
}
const double & lug::Math::Quaternion::operator[](std::size_t idx) const {
    return _valuesArray[idx];
}

//lug::Math::Quaternion lug::Math::Quaternion::conjugate() const {
//    return lug::Math::Quaternion{_x, -_y, -_z, -_w};
//}

//lug::Math::Quaternion lug::Math::Quaternion::invert() {
//    const double numerator = std::pow(_x, 2) + std::pow(_y, 2) + std::pow(_z, 2) + std::pow(_w, 2);
//    return lug::Math::Quaternion{_x / numerator, _y / numerator, _z / numerator, _w / numerator};
//}

//lug::Math::Quaternion lug::Math::Quaternion::normalize() {
//    const double numerator = std::sqrt(std::pow(_x, 2) + std::pow(_y, 2) + std::pow(_z, 2) + std::pow(_w, 2));
//    return lug::Math::Quaternion{_x / numerator, _y / numerator, _z / numerator, _w / numerator};
//}
//
//double lug::Math::Quaternion::angleWith(const Quaternion & otherQuaternion) {
//    lug::Math::Quaternion resQuaternion = otherQuaternion * invert();
//
//
//    return std::move(std::acos(resQuaternion._w) * 2.0);
//}
//
//lug::Math::Quaternion lug::Math::Quaternion::slerp(const Quaternion &otherQuaternion, double t) {
//
//    double angle = angleWith(otherQuaternion) / 2;
//    lug::Math::Quaternion qm = ((*this) * std::sin(1.0 - t) * angle + otherQuaternion * std::sin(t * angle)) / std::sin(angle);
//    return qm;
//}
//
//double lug::Math::Quaternion::getAngle() const {
//    return _angle;
//}
//
//void lug::Math::Quaternion::setAngle(double angle) {
//    _angle = angle;
//}
//


lug::Math::Quaternion lug::Math::operator+(const lug::Math::Quaternion & leftOperand, const lug::Math::Quaternion & rightOperand) {
    return std::move(lug::Math::Quaternion(leftOperand[0] + rightOperand[0], leftOperand[1] + rightOperand[1], leftOperand[2] + rightOperand[2], leftOperand[3] + rightOperand[3]));
}

lug::Math::Quaternion lug::Math::operator-(const lug::Math::Quaternion & leftOperand, const lug::Math::Quaternion & rightOperand) {
    return std::move(lug::Math::Quaternion(leftOperand[0] - rightOperand[0], leftOperand[1] - rightOperand[1], leftOperand[2] - rightOperand[2], leftOperand[3] - rightOperand[3]));
}

lug::Math::Quaternion lug::Math::operator*(const lug::Math::Quaternion & leftOperand, const lug::Math::Quaternion & rightOperand) {
    return std::move(lug::Math::Quaternion(leftOperand[0] * rightOperand[0] - leftOperand[1] * rightOperand[1] - leftOperand[2] * rightOperand[2] - leftOperand[3] * rightOperand[3],
                                            leftOperand[0] * rightOperand[0] + leftOperand[1] * rightOperand[1] + leftOperand[2] * rightOperand[2] - leftOperand[3] * rightOperand[3],
                                            leftOperand[0] * rightOperand[0] - leftOperand[1] * rightOperand[1] + leftOperand[2] * rightOperand[2] + leftOperand[3] * rightOperand[3],
                                            leftOperand[0] * rightOperand[0] + leftOperand[1] * rightOperand[1] - leftOperand[2] * rightOperand[2] + leftOperand[3] * rightOperand[3]));
}

//lug::Math::Quaternion operator/(const lug::Math::Quaternion &leftOperand, const lug::Math::Quaternion &rightOperand) {
//    return std::move(leftOperand * rightOperand.conjugate());
//}


bool lug::Math::operator==(const lug::Math::Quaternion & leftOperand, const lug::Math::Quaternion & rightOperand) {
    if ((leftOperand[0] == rightOperand[0])
        && (leftOperand[1] == rightOperand[1])
        && (leftOperand[2] == rightOperand[2])
        && (leftOperand[3] == rightOperand[3])) {
        return true;
    } else { return false; }
}

lug::Math::Quaternion lug::Math::operator*(const lug::Math::Quaternion & leftOperand, double scalar) {
    lug::Math::Quaternion result(leftOperand);
    result[0] = (result[0] * scalar);
    return result;
}

lug::Math::Quaternion lug::Math::operator+(const lug::Math::Quaternion & leftOperand, double scalar) {
    lug::Math::Quaternion result(leftOperand);
    result[0] = (result[0] + scalar);
    return result;
}

lug::Math::Quaternion lug::Math::operator-(const lug::Math::Quaternion & leftOperand, double scalar) {
    lug::Math::Quaternion result(leftOperand);
    result[0] = (result[0] - scalar);
    return result;
}

lug::Math::Quaternion lug::Math::operator/(const lug::Math::Quaternion & leftOperand, double scalar) {
    lug::Math::Quaternion result(leftOperand);
    result[0] = (result[0] / scalar);
    return result;
}

//lug::Math::Quaternion lug::Math::reflection(const lug::Math::Quaternion & inputePoint, const Vec3d & rotationAxis) {
//    lug::Math::Quaternion in(0, inputePoint.w, inputePoint.x, inputePoint.y);
//    lug::Math::Quaternion q(rotationAxis);
//
//    return std::move(q * in * q);
//}
//
//lug::Math::Quaternion lug::Math::reflection(const lug::Math::Quaternion & inputePoint, const lug::Math::Quaternion & reflectionPlan) {
//    lug::Math::Quaternion in(0, inputePoint.w, inputePoint.x, inputePoint.y);
//
//    lug::Math::Quaternion q(reflectionPlan);
//    q.normalize();
//    q.setX(0.0);
//    return std::move(q * in * q);
//}

/*lug::Math::Quaternion lug::Math::rotation(const Quaternion & inputePoint, double angle, const Vec3d & rotationAxis) {
    double halfAngle = angle / 2.0;
    double sinHalfAngle = std::sin(halfAngle);
    lug::Math::Quaternion q(std::cos(halfAngle), sinHalfAngle * rotationAxis.w, sinHalfAngle * rotationAxis.x, sinHalfAngle * rotationAxis.y);

    return std::move(q * inputePoint * q.conjugate());
}*/
//
//lug::Math::Quaternion lug::Math::rotation(const lug::Math::Quaternion & inputePoint, const lug::Math::Quaternion & q) {
//    return std::move(q * inputePoint * q.conjugate());
//}
