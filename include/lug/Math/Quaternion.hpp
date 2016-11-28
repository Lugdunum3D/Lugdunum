#pragma once

#include <lug/Math/Export.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Math {

template <typename T = double>
class LUG_MATH_API Quaternion {


    public:
    Quaternion(T x, T y, T z, T w);
    Quaternion(T valueArray[4]);
    Quaternion(Vec3d normalizedVector, T angle);

    Quaternion(const Quaternion &otherQuaternion); 
    Quaternion &operator=(const Quaternion &leftOperand);
    virtual ~Quaternion();

    T&  operator[](std::size_t idx);
    const T&  operator[](std::size_t idx) const;

//    Quaternion conjugate() const;
//    Quaternion invert();
//    Quaternion normalize();
//    T angleWith(const Quaternion &otherQuaternion);
//    Quaternion slerp(const Quaternion &otherQuaternion, T t);

 //   T getAngle() const;
//    void setAngle(T angle);

#define DEFINE_QUATERNION_ACCESS(name, rows)                                                                               \
    const T& name() const {                                                                                             \
        return (*this)[rows];                                                                               \
    }                                                                                                                   \
                                                                                                                        \
    T& name() {                                                                                                         \
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



    protected:
//    T _angle;
//    Quaternion *_identity;
//    bool _isIdentity;
//    bool _isNormalized;
   T _valuesArray[4]; // TO DO Transformer en tableau


};
//Quaternion/Quaternion operator
template <typename T >
Quaternion LUG_MATH_API operator+(const Quaternion &leftOperand, const Quaternion &rightOperand);

template <typename T >
Quaternion LUG_MATH_API operator-(const Quaternion &leftOperand, const Quaternion &rightOperand);

template <typename T >
Quaternion LUG_MATH_API operator*(const Quaternion &leftOperand, const Quaternion &rightOperand);

template <typename T >
Quaternion LUG_MATH_API operator/(const Quaternion &leftOperand, const Quaternion &rightOperand);

template <typename T >
bool LUG_MATH_API operator==(const Quaternion &leftOperand, const Quaternion &rightOperand);

//Quaternion/real operator

template <typename T >
Quaternion LUG_MATH_API operator*(const Quaternion &leftOperand, T scalar);

template <typename T >
Quaternion LUG_MATH_API operator+(const Quaternion &leftOperand, T scalar);

template <typename T >
Quaternion LUG_MATH_API operator-(const Quaternion &leftOperand, T scalar);

template <typename T >
Quaternion LUG_MATH_API operator/(const Quaternion &leftOperand, T scalar);


//Formula

template <typename T >
Quaternion LUG_MATH_API reflection(const Quaternion &inputePoint, const Vec3d & rotationAxis);

template <typename T >
Quaternion LUG_MATH_API reflection(const Quaternion &inputePoint, const Quaternion &reflectionPlan);

template <typename T >
Quaternion LUG_MATH_API rotation(const Quaternion &inputePoint, T angle, const Vec3d & rotationAxis);

template <typename T >
Quaternion LUG_MATH_API rotation(const Quaternion &inputePoint, const Quaternion &q);


}
}

#include  <lug/Math/Quaternion.inl>

