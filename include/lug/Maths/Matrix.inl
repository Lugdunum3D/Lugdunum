#include "../Maths/Matrix.hpp"

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline lug::Maths::Matrix<T, dimensionY, dimensionX>::Matrix(const T & unitialValue)
{
	_values.resize(dimensionY);
	for (auto itY = begin(_values); itY != end(_values); ++itY) {
		itY->resize(dimensionX);
		for (auto itX = begin(*itY); itX !=end(*itY); itX++) {
			*itX = unitialValue;
		}
	}
}

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline lug::Maths::Matrix<T, dimensionY, dimensionX>::Matrix(const Matrix<T, dimensionY, dimensionX>& matrix) : _rows(matrix.getRows()), _colums(matrix.getCols()), _values(matrix._values)
{
	
}

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline lug::Maths::Matrix<T, dimensionY, dimensionX>::~Matrix(){}

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline lug::Maths::Matrix<T, dimensionY, dimensionX>& lug::Maths::Matrix<T, dimensionY, dimensionX>::operator=(const Matrix<T, dimensionY, dimensionX>& rightOperand)
{
	if (this == &rightOperand){
		return *this;
	}
	uint8_t newDimensionY = rightOperand.getRows();
	uint8_t newDimensionX = rightOperand.getCols();


	_colums = newDimensionX;
	_rows = newDimensionY;
	_values = MatrixValarray<T>(rightOperand._values);
	return *this;
}

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline uint8_t lug::Maths::Matrix<T, dimensionY, dimensionX>::getRows() const
{
	return _rows;
}
template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline uint8_t lug::Maths::Matrix<T, dimensionY, dimensionX>::getCols() const
{
	return _colums;
}

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline T& lug::Maths::Matrix<T, dimensionY, dimensionX>::operator()(const uint8_t & row, const uint8_t & col)
{
	return _values[row][col];
}

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline const T & lug::Maths::Matrix<T, dimensionY, dimensionX>::operator()(const unsigned & row, const unsigned & col) const
{
	return _values[row][col];
}
/*template<typename T, uint8_t dimensionY, uint8_t dimensionX>
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
			for (uint8_t indexMatrixY = 0; indexMatrixY < dimension; indexMatrixY++) {
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
			det = det + matrix.values[0][indexRowMatrix] * std::pow<T, T>(-1, indexRowMatrix);//* det(minor, dimension - 1);
		}
	}
	return det;
}
*/



