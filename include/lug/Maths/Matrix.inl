#include "../Maths/Matrix.hpp"

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline lug::Maths::Matrix<T, dimensionY, dimensionX>::Matrix() {
	for (uint8_t i = 0; i < dimensionY; i++) {
		for (uint8_t j = 0; j < dimensionX; j++) {
			this->values[i][j] = 0;
		}
	}
}


template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline lug::Maths::Matrix<T, dimensionY, dimensionX>& lug::Maths::Matrix<T, dimensionY, dimensionX>::operator+(
	const lug::Maths::Matrix<T, dimensionY, dimensionX>& rightOperand)
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
inline lug::Maths::Matrix<T, dimensionY, dimensionX>& lug::Maths::Matrix<T, dimensionY, dimensionX>::operator-(
	const lug::Maths::Matrix<T, dimensionY, dimensionX>& rightOperand) {

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
inline lug::Maths::Matrix<T, dimensionY, dimensionX>& lug::Maths::Matrix<T, dimensionY, dimensionX>::operator*(
	const T & number) {
	lug::Maths::Matrix<T, dimensionY, dimensionX> resultMatrix = new lug::Maths::Matrix<T, dimensionY, dimensionX>();
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
inline lug::Maths::Matrix<T, dimensionY, dimensionX>& lug::Maths::Matrix<T, dimensionY, dimensionX>::operator*(
	const lug::Maths::Matrix<T, dimensionY, dimensionX>& rightOperand) {
	lug::Maths::Matrix<T, dimensionY, dimensionX> resultMatrix = new lug::Maths::Matrix<T, dimensionY, dimensionX>();
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
inline void lug::Maths::Matrix<T, dimensionY, dimensionX>::operator=(
	const lug::Maths::Matrix<T, dimensionY, dimensionX>& rightOperand){
	for (uint8_t i = 0; i < dimensionY; i++) {
		for (uint8_t j = 0; j < dimensionX; j++) {
			values[i][j] = rightOperand[i][j];
		}
	}
}



template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline T lug::Maths::Matrix<T, dimensionY, dimensionX>::det(lug::Maths::Matrix<T, dimensionY, dimensionX> matrix, uint8_t dimension)
{
	lug::Maths::Matrix<T, dimensionY - 1, dimensionX - 1> minor = lug::Maths::Matrix<T, dimensionY - 1, dimensionX - 1>();
	uint8_t indexMinorX = 0;
	uint8_t indexMinorY = 0;
	uint8_t indexRowMatrix = 0;
	T det = { 0 };

	if (dimensionX == 1) {
		return matrix.values[0][0];
	}
	else if (dimension == 2) {
		return matrix.values[0][0] * matrix.values[1][1] - matrix.values[0][1] * matrix.values[1][0];
	}
	else {
		for (uint8_t indexRowMatrix = 0; indexRowMatrix < dimension; indexRowMatrix++) {
			indexMinorX = 0;
			indexMinorY = 0;
			for (uint8_t indexMatrixY = 0; indexMatriY < dimension; indexMatrixY++) {
				for (uint8_t indexMatrixX = 0; indexMatrixX < dimension; indexMatrixX++) {
					if (indexMatrixX == indexRowMatrix) {
						continue;
					}
					minor.values[indexMinorY][indexMinorX] = matrix.values[indexMatrixY][indexMatrixX];
					indexMinorX++;
					if (indexMinorX == dimension - 1) {
						indexMinorY++;
						indexMinorX = 0;
					}
				}
			}
			det = det + matrix.values[0][indexRowMatrix] * std::pow<T, T>(-1, indexRowMatrix) * det(minor, dimension - 1);
		}
	}
	return det;
}


template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline lug::Maths::Matrix<T, dimensionY, dimensionX>& lug::Maths::Matrix<T, dimensionY, dimensionX>::operationOnMatrices(
	const lug::Maths::Matrix<T, dimensionY, dimensionX>& rightOperand,
	lug::Maths::Matrix<T, dimensionY, dimensionX>&(*operationFunction)(const T, const T))
{
	lug::Maths::Matrix<T, dimensionY, dimensionX> resultMatrix = new lug::Maths::Matrix<T, dimensionY, dimensionX>();

	for (uint8_t i = 0; i < dimensionY; i++) {
		for (uint8_t j = 0; j < dimensionX; j++) {
			resultMatrix[i][j] = operationFunction(this*, rightOperand);
			j++;
		}
		i++;
	}
	return resultMatrix;
}





