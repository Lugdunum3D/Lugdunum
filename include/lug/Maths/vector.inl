#include "..\..\..\include\lug\Maths\vector.hpp"
/*
template<typename T, uint8_t dimensions>
inline lug::Maths::Vector<T, dimensions>::Vector()
{
}

template<typename T, uint8_t dimensions>
lug::Maths::Vector<T, dimensions>::~Vector()
{
}

template<typename T, uint8_t dimensions>
lug::Maths::Vector<T, dimensions>& lug::Maths::Vector<T, dimensions>::operator+(const lug::Maths::Vector<T, dimensions>& rightOperand)
{
	lug::Maths::Vector<T, dimensions> v = lug::Maths::Vector<T, dimensions>();

	for (uint8_t i = 0; i < dimensions; i++) {
		v.values[i] = this->values[i] + rightOperand.values[i];
	}
	return v;
}

template<typename T, uint8_t dimensions>
lug::Maths::Vector<T, dimensions>& lug::Maths::Vector<T, dimensions>::operator-(const lug::Maths::Vector<T, dimensions>& right)
{
	lug::Maths::Vector<T, dimensions> v = lug::Maths::Vector<T, dimensions>();

	for (uint8_t i = 0; i < dimensions; i++) {
		v.values[i] = this->values[i] - rightOperand.values[i];
	}
	return v;
}

template<typename T, uint8_t dimensions>
lug::Maths::Vector<T, dimensions>& lug::Maths::Vector<T, dimensions>::operator*(const T & number)
{
	lug::Maths::Vector<T, dimensions> v = lug::Maths::Vector<T, dimensions>();

	for (uint8_t i = 0; i < dimensions; i++) {
		v.values[i] = this->values[i] * number;
	}
	return v;
}

template<typename T, uint8_t dimensions>
lug::Maths::Vector<T, dimensions>& lug::Maths::Vector<T, dimensions>::operator^(const Vector<T, dimensions>& right)
{
	return this->wedge(right);
}

template<typename T, uint8_t dimensions>
lug::Maths::Vector<T, dimensions> lug::Maths::Vector<T, dimensions>::wedge(const  lug::Maths::Vector<T, dimensions>& rightOperand)
{
	lug::Maths::Vector<T, dimensions> v = lug::Maths::Vector<T, dimensions>();

	for (uint8_t i = 0; i < dimensions; i++) {
		// TO DO : real calcul for u wedge v
		v.values[i] = this->values[i] * rightOperand.values[i + 1];
	}
	return v;
}

template<typename T, uint8_t dimensions>
T& lug::Maths::Vector<T, dimensions>::dot(const lug::Maths::Vector<T, dimensions>& rightOperand)
{
	T scalarProductResult{ 0 };

	for (uint8_t i = 0; i < dimensions; i++) {
		scalarProductResult = this->values[i] * rightOperand.values[i] + scalarProductResult;
	}
	return scalarProductResult;
}






*/