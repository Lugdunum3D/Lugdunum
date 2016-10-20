#pragma once
#include <cstdint>
#include <cmath>
#include <valarray> 
#include <iostream>

namespace lug {
namespace Maths {


template <typename T>
using MatrixValarray = std::valarray<std::valarray<T>>;

template <typename T, uint8_t rows, uint8_t columns>
class Matrix;

template <typename T, uint8_t rows, uint8_t columns>
std::ostream& operator<<(std::ostream& os, const Matrix<T, rows, columns>& matrix);




template <typename T, uint8_t rows, uint8_t columns>
class Matrix
{
public:
	Matrix(const T& unitialValue);
	Matrix(const Matrix<T, rows, columns>& matrix);
	virtual ~Matrix();

	Matrix<T, rows, columns>& operator=(const Matrix<T, rows, columns>& rightOperand);
	Matrix<T, rows, columns>& operator+=(const Matrix<T, rows, columns>& rightOperand);
	Matrix<T, rows, columns>& operator-=(const Matrix<T, rows, columns>& rightOperand);
	Matrix<T, rows, columns>& operator*=(const Matrix<T, rows, columns>& rightOperand);
	
	Matrix<T, rows, columns> transpose();

	T& operator()(const uint8_t& row, const uint8_t& col);
	const T& operator()(const unsigned& row, const unsigned& col) const;

	static Matrix<T, rows, columns> identity();

	uint8_t getRows() const;
	uint8_t getCols() const;


protected:
	MatrixValarray<T> _values;
	uint8_t _rows;
	uint8_t _colums;

	Matrix<T, rows, columns>& operationOnMatrices(
		const Matrix<T, rows, columns>& rightOperand,
		Matrix<T, rows, columns>& (*operationFunction)(const T, const T));
};

template<typename T, uint8_t rows, uint8_t columns>
std::ostream & operator<<(std::ostream & os, const Matrix<T, rows, columns>& matrix)
{
	for (uint8_t i = 0; i < rows; i++) {
		for (uint8_t j = 0; j < columns; j++) {
			os << matrix(i, j);
		}
		os << std::endl;
	}
	return os;
}

// Matrix/Matrix operation
template <typename T, uint8_t rowsLeft, uint8_t columnsLeft, uint8_t rowsRight, uint8_t columnsRight>
Matrix<T, rows, columns> operator+(const Matrix<T, rowsLeft, columnsLeft>& leftOperand, const Matrix<T, rowsRight, columnsRight>& rightOperand);

template <typename T, uint8_t rows, uint8_t columns>
Matrix<T, rows, columns> operator-(const Matrix<T, rows, columns>& leftOperand, const Matrix<T, rows, columns>& rightOperand);

template <typename T, uint8_t rowsLeft, uint8_t columnsLeft, uint8_t rowsRight, uint8_t columnsRight>
Matrix<T, rows, columns> operator*(const Matrix<T, rowsLeft, columnsLeft>& leftOperand, const Matrix<T, rowsRight, columnsRight>& rightOperand);


// Matrix/scalar operations
template<typename T, uint8_t rows, uint8_t columns>
Matrix<T, rows, columns> operator+(const Matrix<T, rowsLeft, columnsLeft>& matrix, const T& scalar);

template<typename T, uint8_t rows, uint8_t columns>
Matrix<T, rows, columns> operator-(const Matrix<T, rowsLeft, columnsLeft>& matrix, const T& scalar);

template<typename T, uint8_t rows, uint8_t columns>
Matrix<T, rows, columns> operator*(const Matrix<T, rowsLeft, columnsLeft>& matrix, const T& scalar);

template<typename T, uint8_t rows, uint8_t columns>
Matrix<T, rows, columns> operator/(const Matrix<T, rowsLeft, columnsLeft>& matrix, const T& scalar);

// TODO ==operator and != operator

}
}

