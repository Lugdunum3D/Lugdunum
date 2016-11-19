#pragma once

#include <lug/Math/Export.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Math {

class LUG_MATH_API Quaternion {
    public:
    Quaternion(double x, double y, double z, double w);
    Quaternion(Vec3d v);
    Quaternion(const Quaternion &otherQuaternion);
    Quaternion &operator=(const Quaternion &leftOperand);
    virtual ~Quaternion();

    Quaternion conjugate() const;
    Quaternion invert();
    Quaternion normalize();
    double angleWith(const Quaternion &otherQuaternion);
    Quaternion slerp(const Quaternion &otherQuaternion, double t);

    double getAngle() const;
    void setAngle(double angle);

    double getX() const;
    void setX(double x);

    double getY() const;
    void setY(double y);

    double getZ( )const;
    void setZ(double z);

    double getW() const;
    void setW(double w);

    const Vec3d& getAxis() const;
    void setAxis(const Vec3d &);





    const Quaternion &getIdentity() const;

    protected:
    double _angle;
    Vec3d _Axis;
    Quaternion *_identity;
    bool _isIdentity;
    bool _isNormalized;
    double _x, _y, _z, _w; // TO DO Transformer en tableau


};
//Quaternion/Quaternion operator
Quaternion LUG_MATH_API operator+(const Quaternion &leftOperand, const Quaternion &rightOperand);
Quaternion LUG_MATH_API operator-(const Quaternion &leftOperand, const Quaternion &rightOperand);
Quaternion LUG_MATH_API operator*(const Quaternion &leftOperand, const Quaternion &rightOperand);
Quaternion LUG_MATH_API operator/(const Quaternion &leftOperand, const Quaternion &rightOperand);
bool LUG_MATH_API operator==(const Quaternion &leftOperand, const Quaternion &rightOperand);

//Quaternion/real operator
Quaternion LUG_MATH_API operator*(const Quaternion &leftOperand, double scalar);
Quaternion LUG_MATH_API operator+(const Quaternion &leftOperand, double scalar);
Quaternion LUG_MATH_API operator-(const Quaternion &leftOperand, double scalar);
Quaternion LUG_MATH_API operator/(const Quaternion &leftOperand, double scalar);


//Formula
Quaternion LUG_MATH_API reflection(const Quaternion &inputePoint, const Vec3d & rotationAxis);
Quaternion LUG_MATH_API reflection(const Quaternion &inputePoint, const Quaternion &reflectionPlan);
Quaternion LUG_MATH_API rotation(const Quaternion &inputePoint, double angle, const Vec3d & rotationAxis);
Quaternion LUG_MATH_API rotation(const Quaternion &inputePoint, const Quaternion &q);


}
}

