#include "..\..\..\include\lug\Maths\vector.hpp"


template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline Matrix<T, dimensionY, dimensionX>& Matrix<T, dimensionY, dimensionX>::operator+(const Matrix<T, dimensionY, dimensionX>& rightOperand)
{
	Matrix<T, dimensionY, dimensionX> resultMatrix = new Matrix<T, dimensionY, dimensionX>();
	for (uint8_t i = 0; i < dimensionY; i++) {
		for (uint8_t j = 0; j < dimensionX; j++) {
			resultMatrix[i][j] = this->values[i][j] + rightOperand.values[i][j];
			j++;
		}
		i++;
	}
	return resultMatrix;
}
template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline Matrix<T, dimensionY, dimensionX>& Matrix<T, dimensionY, dimensionX>::operator-(const Matrix<T, dimensionY, dimensionX>& rightOperand)
{
	Matrix<T, dimensionY, dimensionX> resultMatrix = new Matrix<T, dimensionY, dimensionX>();
	for (uint8_t i = 0; i < dimensionY; i++) {
		for (uint8_t j = 0; j < dimensionX; j++) {
			resultMatrix[i][j] = this->values[i][j] - rightOperand.values[i][j];
			j++;
		}
		i++;
	}
	return resultMatrix;
}
template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline Matrix<T, dimensionY, dimensionX>& Matrix<T, dimensionY, dimensionX>::operator*(const T & number)
{
	Matrix<T, dimensionY, dimensionX> resultMatrix = new Matrix<T, dimensionY, dimensionX>();
	for (uint8_t i = 0; i < dimensionY; i++) {
		for (uint8_t j = 0; j < dimensionX; j++) {
			resultMatrix[i][j] = this->values[i][j] * number;
			j++;
		}
		i++;
	}
	return resultMatrix;
}
template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline Matrix<T, dimensionY, dimensionX>& Matrix<T, dimensionY, dimensionX>::operator*(const Matrix<T, dimensionY, dimensionX>& rightOperand)
{
	Matrix<T, dimensionY, dimensionX> resultMatrix = new Matrix<T, dimensionY, dimensionX>();
	for (uint8_t i = 0; i < dimensionY; i++) {
		for (uint8_t j = 0; j < dimensionX; j++) {
			resultMatrix[i][j] = this->values[i][i] * rightOperand[i][j] + this->values[i][j] * rightOperand[j][j];
			j++;
		}
		i++;
	}
	return resultMatrix;
}

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline T & Matrix<T, dimensionY, dimensionX>::dot(const Matrix<T, dimensions>& rightOperand) {
	T scalarProductResult{ 0 };

	for (uint8_t i = 0; i < dimensionY; i++) {
		for (uint8_t j = 0; j < dimensionX; j++) {
			scalarProductResult = this->values[i][j] * rightOperand.values[i][j] + scalarProductResult;
		}
	}
	return scalarProductResult;
}