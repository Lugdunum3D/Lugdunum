#pragma once

#include <lug/Math/Export.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Math {

class LUG_MATH_API Quaternion {


    public:
    Quaternion(double x, double y, double z, double w);
    Quaternion(double valueArray[4]);
    Quaternion(Vec3d normalizedVector, double angle);

    Quaternion(const Quaternion &otherQuaternion); 
    Quaternion &operator=(const Quaternion &leftOperand);
    virtual ~Quaternion();

    double&  operator[](std::size_t idx);
    const double&  operator[](std::size_t idx) const;

//    Quaternion conjugate() const;
//    Quaternion invert();
//    Quaternion normalize();
//    double angleWith(const Quaternion &otherQuaternion);
//    Quaternion slerp(const Quaternion &otherQuaternion, double t);

 //   double getAngle() const;
//    void setAngle(double angle);

#define DEFINE_QUATERNION_ACCESS(name, rows)                                                                               \
    const double& name() const {                                                                                             \
        return (*this)[rows];                                                                               \
    }                                                                                                                   \
                                                                                                                        \
    double& name() {                                                                                                         \
        return (*this)[rows];                                                                               \
    }

        DEFINE_QUATERNION_ACCESS(w, 0)
        DEFINE_QUATERNION_ACCESS(i, 0)

        DEFINE_QUATERNION_ACCESS(x, 1)
        DEFINE_QUATERNION_ACCESS(j, 1)

        DEFINE_QUATERNION_ACCESS(y, 2)
        DEFINE_QUATERNION_ACCESS(k, 2)

        DEFINE_QUATERNION_ACCESS(z, 3)
        DEFINE_QUATERNION_ACCESS(l, 3)


#undef DEFINE_QUATERNION_ACCESS







//    const Quaternion &getIdentity() const;

    protected:
//    double _angle;
//    Quaternion *_identity;
//    bool _isIdentity;
//    bool _isNormalized;
   double _valuesArray[4]; // TO DO Transformer en tableau


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

