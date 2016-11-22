#pragma once

#include <Utils.hpp>

#define ASSERT_MATRIX_VALUE_NEAR(row, col, matrix_a, matrix_b, abs_error) ASSERT_NEAR(matrix_a(row, col), matrix_b(row, col), abs_error);

#define ASSERT_MATRIX_COLUMN_NEAR(col, rows, matrix_a, matrix_b, abs_error) REPEAT(rows, ASSERT_MATRIX_VALUE_NEAR, col, matrix_a, matrix_b, abs_error)

#define ASSERT_MATRIX_NEAR(rows, cols, matrix_a, matrix_b, abs_error) EVAL(REPEAT(cols, ASSERT_MATRIX_COLUMN_NEAR, rows, matrix_a, matrix_b, abs_error))
