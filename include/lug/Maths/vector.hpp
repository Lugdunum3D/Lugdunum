#pragma once

#include <cstdint> 
#include "../Maths/Matrix.hpp"
#include <cfloat>

# define M_PI           3.14159265358979323846  /* pi */

namespace lug {
namespace Maths {
template <typename T, uint8_t columns>
class Vector : public Matrix<T, 1, columns> {
public:
	Vector(const T& unitialValue) : Matrix<T, 1, columns>(unitialValue) {};
	Vector(const Vector& otherVector) : Matrix<T, 1, columns>(otherVector) {};
};

template <typename T>
class Vector2 : public Vector<T, 2> {
public:

	static Vector2 RotateWithDegree( const Vector2 &v, const double &degrees);
	static Vector2 RotateWithRadian( const Vector2 &v, const double &radians);

	Vector2(const T& unitialValue) : Vector<T, 2>(unitialValue) {};
	Vector2(const T& newX, const T& newY);
	Vector2(const Vector2& otherVector) : Vector<T, 2>(otherVector) {};
	Vector2(const Vector& otherVector) : Matrix<T, 1, 2>(otherVector) {};

	 T getX() const;
	 T getY() const;

	void setX(const T& newX);
	void setY(const T& newY);


private:
	 const double kDegreeToRadians = M_PI / 180;
};

template <typename T>
class Vector3 : public Vector2<T>
{
public:
	Vector3(const T& unitialValue) : Vector2<T>(unitialValue) {};
	Vector3(const T& newX, const T& newY, const T& newZ);
	Vector3(const Vector3& otherVector) : Vector2<T>(otherVector) {};

	Vector3(const Vector& otherVector) : Matrix<T, 1, 3>(otherVector) {};
	const T& getZ() const;

	void setZ(const T &newZ);

	Vector3<T> operator^(const Vector3<T>& rightOperand);

	Vector3<T> wedge(const Vector3<T>& rightOperand);

	
};

template <typename T, uint8_t columns>
Vector<T, columns> reflection(Vector<T, columns> v, Vector<T, columns> normal);

template <typename T, uint8_t columns>
Vector<T, columns> interpolation(Vector<T, columns> v, Vector<T, columns> u, const T& normal);




}
}

#include "../Maths/vector.inl"