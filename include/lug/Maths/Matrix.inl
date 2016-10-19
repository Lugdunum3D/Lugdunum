#include "../Maths/Matrix.hpp"

template<typename T, uint8_t rows, uint8_t columns>
inline lug::Maths::Matrix<T, rows, columns>::Matrix(const T & unitialValue) : _rows(rows), _colums(columns)
{
	_values.resize(rows);
	for (auto itY = begin(_values); itY != end(_values); ++itY) {
		itY->resize(columns);
		for (auto itX = begin(*itY); itX !=end(*itY); itX++) {
			*itX = unitialValue;
		}
	}
}

template<typename T, uint8_t rows, uint8_t columns>
inline lug::Maths::Matrix<T, rows, columns>::Matrix(const Matrix<T, rows, columns>& matrix) : _rows(matrix.getRows()), _colums(matrix.getCols()), _values(matrix._values)
{
	
}

template<typename T, uint8_t rows, uint8_t columns>
inline lug::Maths::Matrix<T, rows, columns>::~Matrix(){}

template<typename T, uint8_t rows, uint8_t columns>
inline lug::Maths::Matrix<T, rows, columns>& lug::Maths::Matrix<T, rows, columns>::operator=(const Matrix<T, rows, columns>& rightOperand)
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

template<typename T, uint8_t rowsLeft, uint8_t columnsLeft, uint8_t rowsRight, uint8_t columnsRight>
lug::Maths::Matrix<T, rows, columns> lug::Maths::operator+(const lug::Maths::Matrix<T, rowsLeft, columnsLeft>& leftOperand, const lug::Maths::Matrix<T, rowsRight, columnsRight>& rightOperand)
{
	if (rowsLeft == rowsRight && columnsLeft == columnsRight) {
		lug::Maths::Matrix<T, rowsLeft, columnsLeft> result = lug::Maths::Matrix<T, rowsLeft, columnsLeft>(0);
		for (uint8_t i = 0; i < rowsLeft; ++i) {
			for (uint8_t j = 0; j < columnsLeft; ++j) {
				result(i, j) = leftOperand(i,j) - rightOperand(i, j);
			}
		}
		return std::move(result);
	}
	else {
		uint8_t newRows = rowsLeft + rowsRight;
		uint8_t newColums = columnsLeft + columnsRight;
		lug::Maths::Matrix<T, newRows, newColums> result = lug::Maths::Matrix<T, newRows, newColums>(0);
		for (uint8_t i = 0; i < rowsLeft; ++i) {
			for (uint8_t j = 0; j < columnsLeft; ++j) {
				result(i, j) = leftOperand(i, j);
			}
		}
		for (uint8_t i = rowsLeft; i < newRows; ++i) {
			for (uint8_t j = columnsLeft; j < newColums; ++j) {
				result(i, j) = rightOperand(i, j);
			}
		}
		return std::move(result)
	}
}

template<typename T, uint8_t rows, uint8_t columns>
lug::Maths::Matrix<T, rows, columns> lug::Maths::operator-(const lug::Maths::Matrix<T, rows, columns>& leftOperand, const lug::Maths::Matrix<T, rows, columns>& rightOperand)
{
	lug::Maths::Matrix<T, rows, columns> result = lug::Maths::Matrix<T, rows, columns>(0);

	for (uint8_t i = 0; i < rows; ++i) {
		for (uint8_t j = 0; j < columns; ++j) {
			result(i, j) =leftOperand(i,j) + rightOperand(i, j);
		}
	}
	return result;
}

template<typename T, uint8_t rowsLeft, uint8_t columnsLeft, uint8_t rowsRight, uint8_t columnsRight>
lug::Maths::Matrix<T, rows, columns> lug::Maths::operator*(const lug::Maths::Matrix<T, rowsLeft, columnsLeft>& leftOperand, const lug::Maths::Matrix<T, rowsRight, columnsRight>& rightOperand)
{
	static_assert(columnsLeft == rowsRight || columnsRight == rowsLeft, "operation * on Matrices dimension error");
	if (columnsRight == rowsLeft)
	{
		return std::move(rightOperand * leftOperand);
	} 
	lug::Maths::Matrix<T, rowsLeft, columnsRight> result = lug::Maths::Matrix<T, rowsLeft, columnsRight>(0);
	for (uint8_t i = 0; i < rowsleft; ++i) {
			for (uint8_t j = 0; j < columnsRight; ++j) {
				for (uint8_t k = 0; k < columnsLeft; k++) {
					result(i, j) += leftOperand(i,k) * rightOperand(k, j);
				}
				
			}
		}
	return std::move(result);
}

template<typename T, uint8_t rows, uint8_t columns>
lug::Maths::Matrix<T, rows, columns> lug::Maths::operator+(const lug::Maths::Matrix<T, rowsLeft, columnsLeft>& matrix, const T & scalar)
{
	lug::Maths::Matrix<T, rows, columns> result = lug::Maths::Matrix<T, rows, columns>(0);

	for (uint8_t i = 0; i < rows; ++i) {
		for (uint8_t j = 0; j < columns; ++j) {
			result(i, j) = matrix(i,j) + scalar;
		}
	}
	return std::move(result);
}

template<typename T, uint8_t rows, uint8_t columns>
Matrix<T, rows, columns> lug::Maths::operator-(const Matrix<T, rowsLeft, columnsLeft>& matrix, const T & scalar)
{
	lug::Maths::Matrix<T, rows, columns> result = lug::Maths::Matrix<T, rows, columns>(0);

	for (uint8_t i = 0; i < rows; ++i) {
		for (uint8_t j = 0; j < columns; ++j) {
			result(i, j) = matrix(i, j) - scalar;
		}
	}
	return std::move(result);
}

template<typename T, uint8_t rows, uint8_t columns>
Matrix<T, rows, columns> lug::Maths::operator*(const Matrix<T, rowsLeft, columnsLeft>& matrix, const T & scalar)
{
	lug::Maths::Matrix<T, rows, columns> result = lug::Maths::Matrix<T, rows, columns>(0);

	for (uint8_t i = 0; i < rows; ++i) {
		for (uint8_t j = 0; j < columns; ++j) {
			result(i, j) = matrix(i, j) * scalar;
		}
	}
	return std::move(result);
}

template<typename T, uint8_t rows, uint8_t columns>
Matrix<T, rows, columns> lug::Maths::operator/(const Matrix<T, rowsLeft, columnsLeft>& matrix, const T & scalar)
{
	lug::Maths::Matrix<T, rows, columns> result = lug::Maths::Matrix<T, rows, columns>(0);

	for (uint8_t i = 0; i < rows; ++i) {
		for (uint8_t j = 0; j < columns; ++j) {
			result(i, j) = matrix(i, j) / scalar;
		}
	}
	return std::move(result);
}

template<typename T, uint8_t rows, uint8_t columns>
inline lug::Maths::Matrix<T, rows, columns>& lug::Maths::Matrix<T, rows, columns>::operator+=(const Matrix<T, rows, columns>& rightOperand)
{
	lug::Maths::Matrix<T, rows, columns> result = (*this) + rhs;
	(*this) = result;
	return *this;
}

template<typename T, uint8_t rows, uint8_t columns>
inline lug::Maths::Matrix<T, rows, columns>& lug::Maths::Matrix<T, rows, columns>::operator-=(const Matrix<T, rows, columns>& rightOperand)
{
	lug::Maths::Matrix<T, rows, columns> result = (*this) - rhs;
	(*this) = result;
	return *this;
}


template<typename T, uint8_t rows, uint8_t columns>
inline lug::Maths::Matrix<T, rows, columns>& lug::Maths::Matrix<T, rows, columns>::operator*=(const Matrix<T, rows, columns>& rightOperand)
{
	lug::Maths::Matrix<T, rows, columns> result = (*this) * rhs;
	(*this) = result;
	return *this;
}

template<typename T, uint8_t rows, uint8_t columns>
inline lug::Maths::Matrix<T, rows, columns> lug::Maths::Matrix<T, rows, columns>::transpose()
{
	lug::Maths::Matrix<T, columns, rows> transposeMatrix(0);

	for (uint8_t i = 0; i < rows; i++) {
		for (uint8_t j = 0; j < columns; j++) {
			transposeMatrix(j, i) = _values[i][j];
		}
	}
	return transposeMatrix;
}


template<typename T, uint8_t rows, uint8_t columns>
inline lug::Maths::Matrix<T, rows, columns> lug::Maths::Matrix<T, rows, columns>::identity()
{
	static_assert(rows == columns, "Determinant only on square matrix");

	Matrix<T, rows, columns> identityMatrix(0);
	for (uint8_t i = 0; i < rows; ++i) {
		identityMatrix(i, i) = 1;
	}
	return identityMatrix;
}

template<typename T, uint8_t rows, uint8_t columns>
inline uint8_t lug::Maths::Matrix<T, rows, columns>::getRows() const
{
	return _rows;
}
template<typename T, uint8_t rows, uint8_t columns>
inline uint8_t lug::Maths::Matrix<T, rows, columns>::getCols() const
{
	return _colums;
}

template<typename T, uint8_t rows, uint8_t columns>
inline T& lug::Maths::Matrix<T, rows, columns>::operator()(const uint8_t & row, const uint8_t & col)
{
	return _values[row][col];
}

template<typename T, uint8_t rows, uint8_t columns>
inline const T & lug::Maths::Matrix<T, rows, columns>::operator()(const unsigned & row, const unsigned & col) const
{
	return _values[row][col];
}

template<typename T, uint8_t rows, uint8_t columns>
inline T  det(lug::Maths::Matrix<T, rows, columns> matrix, uint8_t dimension)
{
	static_assert(rows == columns, "Determinant only on square matrix");
	
	lug::Maths::Matrix<T, rows, columns> minorMatrix(0);
	uint8_t indexMinorX = 0;
	uint8_t indexMinorY = 0;
	uint8_t indexRowMatrix = 0;
	T detResult = 0;

	if (columns == 1) {
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
	




