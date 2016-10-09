#pragma once
#include <cstdint>
#include <cmath>

namespace lug {
namespace Maths {
	template <typename T, uint8_t dimensionY, uint8_t dimensionX>
	class Matrix
	{
	public:
		Matrix();
		~Matrix() {};

		T values[dimensionY][dimensionX];

		Matrix<T, dimensionY, dimensionX>& operator+(const Matrix<T, dimensionY, dimensionX>& rightOperand);
		Matrix<T, dimensionY, dimensionX>& operator-(const Matrix<T, dimensionY, dimensionX>& rightOperand);
		Matrix<T, dimensionY, dimensionX>& operator*(const T& number);
		Matrix<T, dimensionY, dimensionX>& operator*(const Matrix<T, dimensionY, dimensionX>& rightOperand);
		void operator=(const Matrix<T, dimensionY, dimensionX>& rightOperand);

		T det(lug::Maths::Matrix<T, dimensionY, dimensionX> matrix, uint8_t dimension);

		static Matrix<T, dimensionY, dimensionX>& identity();

	private:
		Matrix<T, dimensionY, dimensionX>& operationOnMatrices(const Matrix<T, dimensionY, dimensionX>& rightOperand, Matrix<T, dimensionY, dimensionX>& (*operationFunction)(const T, const T));
		T cofatctor(uint8_t index);
	};

}
}

