#pragma once
#include <cstdint>
#include <cmath>
#include <valarray> 
#include <iostream>

namespace lug {
namespace Maths {

	template <typename T>
	using MatrixValarray = std::valarray<std::valarray<T>>;

	template <typename T, uint8_t dimensionY, uint8_t dimensionX>
	class Matrix;

	template <typename T, uint8_t dimensionY, uint8_t dimensionX>
	std::ostream& operator<<(std::ostream& os, const Matrix<T, dimensionY, dimensionX>& matrix);

	template <typename T, uint8_t dimensionY, uint8_t dimensionX>
	class Matrix
	{
	public:
		Matrix(const T& unitialValue);
		Matrix(const Matrix<T, dimensionY, dimensionX>& matrix);
		virtual ~Matrix();

		Matrix<T, dimensionY, dimensionX>& operator=(const Matrix<T, dimensionY, dimensionX>& rightOperand);
		
		Matrix<T, dimensionY, dimensionX> operator+(const Matrix<T, dimensionY, dimensionX>& rightOperand);
		Matrix<T, dimensionY, dimensionX> operator-(const Matrix<T, dimensionY, dimensionX>& rightOperand);
		Matrix<T, dimensionY, dimensionX>& operator+=(const Matrix<T, dimensionY, dimensionX>& rightOperand);
		Matrix<T, dimensionY, dimensionX>& operator-=(const Matrix<T, dimensionY, dimensionX>& rightOperand);
		Matrix<T, dimensionY, dimensionX> operator*(const Matrix<T, dimensionY, dimensionX>& rightOperand);
		Matrix<T, dimensionY, dimensionX>& operator*=(const Matrix<T, dimensionY, dimensionX>& rightOperand);
		Matrix<T, dimensionY, dimensionX> transpose();

		// Matrix/scalar operations
		Matrix<T, dimensionY, dimensionX> operator+(const T& scalar);
		Matrix<T, dimensionY, dimensionX> operator-(const T& scalar);
		Matrix<T, dimensionY, dimensionX> operator*(const T& scalar);
		Matrix<T, dimensionY, dimensionX> operator/(const T& scalar);
		
		T& operator()(const uint8_t& row, const uint8_t& col);
		const T& operator()(const unsigned& row, const unsigned& col) const;
		

		T det(lug::Maths::Matrix<T, dimensionY, dimensionX> matrix, uint8_t dimension);

		static Matrix<T, dimensionY, dimensionX> identity();

		uint8_t getRows() const;
		uint8_t getCols() const;


	private:
		MatrixValarray<T> _values;
		uint8_t _rows;
		uint8_t _colums;

		Matrix<T, dimensionY, dimensionX>& operationOnMatrices(
			const Matrix<T, dimensionY, dimensionX>& rightOperand,
			Matrix<T, dimensionY, dimensionX>& (*operationFunction)(const T, const T));
	};

	template<typename T, uint8_t dimensionY, uint8_t dimensionX>
	std::ostream & operator<<(std::ostream & os, const Matrix<T, dimensionY, dimensionX>& matrix)
	{
		for (uint8_t i = 0; i < dimensionY; i++) {
			for (uint8_t j = 0; j < dimensionX; j++) {
				os << matrix(i, j);
			}
			os << std::endl;
		}
		return os;
	}
	
}
}

