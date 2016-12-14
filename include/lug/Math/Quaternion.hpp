#pragma once

#include <lug/Math/Export.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Math {

template <typename T = double>
class Quaternion {


public:
    Quaternion(T x, T y, T z, T w);
    Quaternion(T valueArray[4]);
    Quaternion(Vector<3, T> normalizedVector, T angle);

    Quaternion(const Quaternion<T> &otherQuaternion);
    Quaternion<T> &operator=(const Quaternion<T> &leftOperand);
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
Quaternion<T>  operator+(const Quaternion<T> &leftOperand, const Quaternion<T> &rightOperand);

template <typename T >
Quaternion<T> operator-(const Quaternion<T> &leftOperand, const Quaternion<T> &rightOperand);

template <typename T >
Quaternion<T> operator*(const Quaternion<T> &leftOperand, const Quaternion<T> &rightOperand);

template <typename T >
Quaternion<T> operator/(const Quaternion<T> &leftOperand, const Quaternion<T> &rightOperand);

template <typename T >
bool  operator==(const Quaternion<T> &leftOperand, const Quaternion<T> &rightOperand);

//Quaternion/real operator

template <typename T >
Quaternion<T> operator*(const Quaternion<T> &leftOperand, T scalar);

template <typename T >
Quaternion<T> operator+(const Quaternion<T> &leftOperand, T scalar);

template <typename T >
Quaternion<T> operator-(const Quaternion<T> &leftOperand, T scalar);

template <typename T >
Quaternion<T> operator/(const Quaternion<T> &leftOperand, T scalar);


//Formula

template <typename T >
Quaternion<T> reflection(const Quaternion<T> &inputePoint, const Vec3d & rotationAxis);

template <typename T >
Quaternion<T> reflection(const Quaternion<T> &inputePoint, const Quaternion<T> &reflectionPlan);

template <typename T >
Quaternion<T> rotation(const Quaternion<T> &inputePoint, T angle, const lug::Math::Vector<3, T> & rotationAxis);

template <typename T >
Quaternion<T> rotation(const Quaternion<T> &inputePoint, const Quaternion<T> &q);


}
}

#include  <lug/Math/Quaternion.inl>

