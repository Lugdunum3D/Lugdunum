#include "../Maths/Matrix.hpp"

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline lug::Maths::Matrix<T, dimensionY, dimensionX>::Matrix(const T & unitialValue) : _rows(dimensionY), _colums(dimensionX)
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
inline lug::Maths::Matrix<T, dimensionY, dimensionX> lug::Maths::Matrix<T, dimensionY, dimensionX>::operator+(const Matrix<T, dimensionY, dimensionX>& rightOperand)
{
	lug::Maths::Matrix<T, dimensionY, dimensionX> result = lug::Maths::Matrix<T, dimensionY, dimensionX>(0);

	for (uint8_t i = 0; i < dimensionY; ++i) {
		for (uint8_t j = 0; j < dimensionX; ++j) {
			result(i, j) = _values[i][j] + rightOperand(i, j);
		}
	}
	return result;
}

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline lug::Maths::Matrix<T, dimensionY, dimensionX> lug::Maths::Matrix<T, dimensionY, dimensionX>::operator-(const Matrix<T, dimensionY, dimensionX>& rightOperand)
{
	lug::Maths::Matrix<T, dimensionY, dimensionX> result = lug::Maths::Matrix<T, dimensionY, dimensionX>(0);

	for (uint8_t i = 0; i < dimensionY; ++i) {
		for (uint8_t j = 0; j < dimensionX; ++j) {
			result(i, j) = _values[i][j] - rightOperand(i, j);
		}
	}
	return result;
}

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline lug::Maths::Matrix<T, dimensionY, dimensionX>& lug::Maths::Matrix<T, dimensionY, dimensionX>::operator+=(const Matrix<T, dimensionY, dimensionX>& rightOperand)
{
	lug::Maths::Matrix<T, dimensionY, dimensionX> result = (*this) + rhs;
	(*this) = result;
	return *this;
}

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline lug::Maths::Matrix<T, dimensionY, dimensionX>& lug::Maths::Matrix<T, dimensionY, dimensionX>::operator-=(const Matrix<T, dimensionY, dimensionX>& rightOperand)
{
	lug::Maths::Matrix<T, dimensionY, dimensionX> result = (*this) - rhs;
	(*this) = result;
	return *this;
}

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline lug::Maths::Matrix<T, dimensionY, dimensionX> lug::Maths::Matrix<T, dimensionY, dimensionX>::operator*(const Matrix<T, dimensionY, dimensionX>& rightOperand)
{
	lug::Maths::Matrix<T, dimensionY, dimensionX> result = lug::Maths::Matrix<T, dimensionY, dimensionX>(0);

	for (uint8_t i = 0; i < dimensionY; ++i) {
		for (uint8_t j = 0; j < dimensionX; ++j) {
			result(i, j) = _values[i][j] * rightOperand(i, j);
		}
	}
	return result;
}

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline lug::Maths::Matrix<T, dimensionY, dimensionX>& lug::Maths::Matrix<T, dimensionY, dimensionX>::operator*=(const Matrix<T, dimensionY, dimensionX>& rightOperand)
{
	lug::Maths::Matrix<T, dimensionY, dimensionX> result = (*this) * rhs;
	(*this) = result;
	return *this;
}

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline lug::Maths::Matrix<T, dimensionY, dimensionX> lug::Maths::Matrix<T, dimensionY, dimensionX>::transpose()
{
	lug::Maths::Matrix<T, dimensionX, dimensionY> transposeMatrix(0);

	for (uint8_t i = 0; i < dimensionY; i++) {
		for (uint8_t j = 0; j < dimensionX; j++) {
			transposeMatrix(j, i) = _values[i][j];
		}
	}
	return transposeMatrix;
}

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline lug::Maths::Matrix<T, dimensionY, dimensionX> lug::Maths::Matrix<T, dimensionY, dimensionX>::operator+(const T & scalar)
{
	lug::Maths::Matrix<T, dimensionY, dimensionX> result = lug::Maths::Matrix<T, dimensionY, dimensionX>(0);

	for (uint8_t i = 0; i < dimensionY; ++i) {
		for (uint8_t j = 0; j < dimensionX; ++j) {
			result(i, j) = _values[i][j] + scalar;
		}
	}
	return result;
}

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline lug::Maths::Matrix<T, dimensionY, dimensionX> lug::Maths::Matrix<T, dimensionY, dimensionX>::operator-(const T & scalar)
{
	lug::Maths::Matrix<T, dimensionY, dimensionX> result = lug::Maths::Matrix<T, dimensionY, dimensionX>(0);

	for (uint8_t i = 0; i < dimensionY; ++i) {
		for (uint8_t j = 0; j < dimensionX; ++j) {
			result(i, j) = _values[i][j] - scalar;
		}
	}
	return result;
}

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline lug::Maths::Matrix<T, dimensionY, dimensionX> lug::Maths::Matrix<T, dimensionY, dimensionX>::operator*(const T & scalar)
{
	lug::Maths::Matrix<T, dimensionY, dimensionX> result = lug::Maths::Matrix<T, dimensionY, dimensionX>(0);

	for (uint8_t i = 0; i < dimensionY; ++i) {
		for (uint8_t j = 0; j < dimensionX; ++j) {
			result(i, j) = _values[i][j] * scalar;
		}
	}
	return result;
}

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline  lug::Maths::Matrix<T, dimensionY, dimensionX> lug::Maths::Matrix<T, dimensionY, dimensionX>::operator/(const T & scalar)
{
	lug::Maths::Matrix<T, dimensionY, dimensionX> result = lug::Maths::Matrix<T, dimensionY, dimensionX>(0);

	for (uint8_t i = 0; i < dimensionY; ++i) {
		for (uint8_t j = 0; j < dimensionX; ++j) {
			result(i, j) = _values[i][j] / scalar;
		}
	}
	return result;
}



template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline lug::Maths::Matrix<T, dimensionY, dimensionX> lug::Maths::Matrix<T, dimensionY, dimensionX>::identity()
{
	static_assert(dimensionY == dimensionX, "Determinant only on square matrix");

	Matrix<T, dimensionY, dimensionX> identityMatrix(0);
	for (uint8_t i = 0; i < dimensionY; ++i) {
		identityMatrix(i, i) = 1;
	}
	return identityMatrix;
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

template<typename T, uint8_t dimensionY, uint8_t dimensionX>
inline T  det(lug::Maths::Matrix<T, dimensionY, dimensionX> matrix, uint8_t dimension)
{
	static_assert(dimensionY == dimensionX, "Determinant only on square matrix");
	
	lug::Maths::Matrix<T, dimensionY, dimensionX> minorMatrix(0);
	uint8_t indexMinorX = 0;
	uint8_t indexMinorY = 0;
	uint8_t indexRowMatrix = 0;
	T detResult = 0;

	if (dimensionX == 1) {
		return matrix(0, 0);
	}
	else if (dimension == 2) {
		return matrix(0,0) * matrix(1,1) - matrix(0, 1) * matrix(1, 0);
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
					minorMatrix(indexMinorY, indexMinorX) = matrix(indexMatrixY, indexMatrixX);
					indexMinorX++;
					if (indexMinorX == dimension - 1) {
						indexMinorY++;
						indexMinorX = 0;
					}
				}
			}
			detResult = detResult + matrix(0, indexRowMatrix) * std::pow(-1, indexRowMatrix) * det(minorMatrix, dimension - 1);
		}
		return detResult;
	}
	
}
	




