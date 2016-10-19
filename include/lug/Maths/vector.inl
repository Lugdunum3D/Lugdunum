#include "..\..\..\include\lug\Maths\vector.hpp"

template<typename T>
inline lug::Maths::Vector2<T> lug::Maths::Vector2<T>::RotateWithDegree(const lug::Maths::Vector2<T> & v, const double & degrees)
{
	return RotateWithRadians(v, kDegreeToRadians * degrees);
}

template<typename T>
inline lug::Maths::Vector2<T> lug::Maths::Vector2<T>::RotateWithRadian(const lug::Maths::Vector2<T> & v, const double & radians)
{
	double cosA = std::cos(radians);
	double sinA = std::sin(radians);
	return Vector2(cosA * v.getX - sinA * v.getY, sinA * v.getX + cosA * v.getY);

}

template<typename T>
inline lug::Maths::Vector2<T>::Vector2(const T & newX, const T & newY) : _rows(1), _columns(2)
{
	_values[0][0] = newX;
	_values[0][1] = newY;
}

template<typename T>
inline  T lug::Maths::Vector2<T>::getX() const
{
	return _values[0][0];
}

template<typename T>
inline T  lug::Maths::Vector2<T>::getY() const
{
	return _values[0][1];
}

template<typename T>
inline void lug::Maths::Vector2<T>::setX(const T& newX)
{
	_value[0][0] = newX;
}

template<typename T>
inline void lug::Maths::Vector2<T>::setY(const T & newY)
{
	_values[0][1] = newY;
}


template<typename T>
inline lug::Maths::Vector3<T>::Vector3(const T & newX, const T & newY, const T & newZ) : lug::Maths::Vector2<T>(newX, newY)
{
	_values[0][2] = newZ;
}

template<typename T>
inline const T & lug::Maths::Vector3<T>::getZ() const
{
	return _values[0][2]
}

template<typename T>
inline void lug::Maths::Vector3<T>::setZ(const T & newZ)
{
	_values[0][2] = newZ;
}

template<typename T>
inline  lug::Maths::Vector3<T> lug::Maths::Vector3<T>::operator^(const  lug::Maths::Vector3<T>& rightOperand)
{
	return wedge(rightOperand);
}

template<typename T>
inline lug::Maths::Vector3<T> lug::Maths::Vector3<T>::wedge(const lug::Maths::Vector3<T>& rightOperand)
{
	return lug::Maths::Vector3<T>(
		this.getY() * rightOperand.getZ() - this->getZ() * rightOperand.getY(),
		this.getZ() * rightOperand.getX() - this->getX() * rightOperand.getZ(),
		this.getX() * rightOperand.getY() - this->getY() * rightOperand.getX());
}

template<typename T, uint8_t columns>
inline lug::Maths::Vector<T, columns> lug::Maths::reflection(lug::Maths::Vector<T, columns> v, lug::Maths::Vector<T, columns> normal)
{

	return v - normal *((v * normal) * 2);
}

template<typename T, uint8_t columns>
inline lug::Maths::Vector<T, columns> lug::Maths::interpolation(Vector<T, columns> v, Vector<T, columns> u, const T & normal)
{
	return v + ratio * (u - v);
}
