#include <gtest/gtest.h>
#include <lug/Math/Matrix.hpp>
#include <lug/Math/Vector.hpp>

// TODO: Really test all operators and values
// TODO: Test the determinant
// TODO: Test the transpose

TEST(Matrix3x3, Constructors) {
    lug::Math::Mat3x3i matrix{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    ASSERT_EQ(matrix(0, 0), 1);
    ASSERT_EQ(matrix(0, 1), 2);
    ASSERT_EQ(matrix(0, 2), 3);
    ASSERT_EQ(matrix(1, 0), 4);
    ASSERT_EQ(matrix(1, 1), 5);
    ASSERT_EQ(matrix(1, 2), 6);
    ASSERT_EQ(matrix(2, 0), 7);
    ASSERT_EQ(matrix(2, 1), 8);
    ASSERT_EQ(matrix(2, 2), 9);
}

TEST(Matrix3x3, GeneralOperators) {
    lug::Math::Mat3x3f matrixL(1.0f);
    lug::Math::Mat3x3f matrixM(1.0f);

    lug::Math::Vec3f vectorU(1.0f);
    lug::Math::Vec3f vectorV(1.0f);

    float x = 1.0f;

    lug::Math::Vec3f vectorA = matrixM * vectorU;
    lug::Math::Vec3f vectorB = vectorV * matrixM;

    lug::Math::Mat3x3f matrixN = x / matrixM;
    lug::Math::Mat3x3f matrixO = matrixM / x;
    lug::Math::Mat3x3f matrixP = x * matrixM;
    lug::Math::Mat3x3f matrixQ = matrixM * x;

    EXPECT_TRUE(!(matrixM != matrixQ));
    EXPECT_TRUE(matrixM == matrixL);
}

TEST(Matrix3x3, InverseOperators) {
    const lug::Math::Mat3x3f goodIdentity = lug::Math::Mat3x3f::identity();

    {
        const lug::Math::Mat3x3f matrix{
            0.6f, 0.2f, 0.3f,
            0.2f, 0.7f, 0.5f,
            0.3f, 0.5f, 0.7f
        };
        const lug::Math::Mat3x3f inverse = matrix.inverse();
        const lug::Math::Mat3x3f identity = matrix * inverse;

        for (uint8_t row = 0; row < matrix.getRows(); ++row) {
            for (uint8_t col = 0; col < matrix.getColumns(); ++col) {
                ASSERT_NEAR(identity(row, col), goodIdentity(row, col), 0.01f)
                    << "row = " << static_cast<int>(row) << "\n"
                    << "col = " << static_cast<int>(col);
            }
        }
    }

    {
        const lug::Math::Mat3x3f matrix{
            0.6f, 0.2f, 0.3f,
            0.2f, 0.7f, 0.5f,
            0.3f, 0.5f, 0.7f
        };
        const lug::Math::Mat3x3f identity = matrix / matrix;

        for (uint8_t row = 0; row < matrix.getRows(); ++row) {
            for (uint8_t col = 0; col < matrix.getColumns(); ++col) {
                ASSERT_NEAR(identity(row, col), goodIdentity(row, col), 0.01f)
                    << "row = " << static_cast<int>(row) << "\n"
                    << "col = " << static_cast<int>(col);
            }
        }
    }
}

TEST(Matrix3x3, Size) {
    ASSERT_EQ(lug::Math::Mat3x3f().getRows(), 3);
    ASSERT_EQ(lug::Math::Mat3x3f().getColumns(), 3);
}

TEST(Matrix3x3, Addition) {
    lug::Math::Mat3x3f matrixA{
        2.f, -6.f, 8.f,
        7.f,  8.f, 2.f,
        4.f,  3.f, 7.f
    };
    lug::Math::Mat3x3f matrixB{
         1.f, 5.f, 4.f,
        -7.f, 3.f, 6.f,
         5.f, 2.f, 8.f
    };

    // Operator Tests
    {
        const lug::Math::Mat3x3f result = matrixA + matrixB;

        const lug::Math::Mat3x3f correctResult{
            3.f, -1.f,  12.f,
            0.f,  11.f, 8.f,
            9.f,  5.f,  15.f
        };

        for (uint8_t row = 0; row < result.getRows(); ++row) {
            for (uint8_t col = 0; col < result.getColumns(); ++col) {
                ASSERT_NEAR(result(row, col), correctResult(row, col), 0.01f)
                    << "row = " << static_cast<int>(row) << "\n"
                    << "col = " << static_cast<int>(col);
            }
        }

        matrixA += matrixB;

        for (uint8_t row = 0; row < matrixA.getRows(); ++row) {
            for (uint8_t col = 0; col < matrixA.getColumns(); ++col) {
                ASSERT_NEAR(matrixA(row, col), correctResult(row, col), 0.01f)
                    << "row = " << static_cast<int>(row) << "\n"
                    << "col = " << static_cast<int>(col);
            }
        }
    }

    // Scalar test
    {
        const lug::Math::Mat3x3f result = matrixB + 1.0f;

        const lug::Math::Mat3x3f correctResult{
            1.f + 1.0f,  5.f + 1.0f, 4.f + 1.0f,
           -7.f + 1.0f,  3.f + 1.0f, 6.f + 1.0f,
            5.f + 1.0f,  2.f + 1.0f, 8.f + 1.0f
        };

        for (uint8_t row = 0; row < result.getRows(); ++row) {
            for (uint8_t col = 0; col < result.getColumns(); ++col) {
                ASSERT_NEAR(result(row, col), correctResult(row, col), 0.01f)
                    << "row = " << static_cast<int>(row) << "\n"
                    << "col = " << static_cast<int>(col);
            }
        }

    }
}

TEST(Matrix3x3, Substraction) {
    lug::Math::Mat3x3f matrixA{
        2.f, -6.f, 8.f,
        7.f,  8.f, 2.f,
        4.f,  3.f, 7.f
    };
    lug::Math::Mat3x3f matrixB{
         1.f, 5.f, 4.f,
        -7.f, 3.f, 6.f,
         5.f, 2.f, 8.f
    };

    // Operator Tests
    {
        const lug::Math::Mat3x3f result = matrixA - matrixB;

        const lug::Math::Mat3x3f correctResult{
             1.f, -11.f, 4.f,
             14.f, 5.f, -4.f,
            -1.f,  1.f, -1.f
        };

        for (uint8_t row = 0; row < result.getRows(); ++row) {
            for (uint8_t col = 0; col < result.getColumns(); ++col) {
                ASSERT_NEAR(result(row, col), correctResult(row, col), 0.01f)
                    << "row = " << static_cast<int>(row) << "\n"
                    << "col = " << static_cast<int>(col);
            }
        }

        matrixA -= matrixB;

        for (uint8_t row = 0; row < matrixA.getRows(); ++row) {
            for (uint8_t col = 0; col < matrixA.getColumns(); ++col) {
                ASSERT_NEAR(matrixA(row, col), correctResult(row, col), 0.01f)
                    << "row = " << static_cast<int>(row) << "\n"
                    << "col = " << static_cast<int>(col);
            }
        }
    }

    // Scalar Tests
    {
        const lug::Math::Mat3x3f result = matrixB - 1.f;

        const lug::Math::Mat3x3f correctResult{
            1.f - 1.f,  5.f - 1.f, 4.f - 1.f,
            -7.f - 1.f,  3.f - 1.f, 6.f - 1.f,
            5.f - 1.f,  2.f - 1.f, 8.f - 1.f
        };

        for (uint8_t row = 0; row < result.getRows(); ++row) {
            for (uint8_t col = 0; col < result.getColumns(); ++col) {
                ASSERT_NEAR(result(row, col), correctResult(row, col), 0.01f)
                    << "row = " << static_cast<int>(row) << "\n"
                    << "col = " << static_cast<int>(col);
            }
        }
    }

}

TEST(Matrix3x3, Multiplication) {
    lug::Math::Mat3x3f matrixA{
        2.f, -6.f, 8.f,
        7.f,  8.f, 2.f,
        4.f,  3.f, 7.f
    };
    lug::Math::Mat3x3f matrixB{
         1.f, 5.f, 4.f,
        -7.f, 3.f, 6.f,
         5.f, 2.f, 8.f
    };

    // Operator Tests
    {
        const lug::Math::Mat3x3f result = matrixA * matrixB;

        const lug::Math::Mat3x3f correctResult{
             84.f,  8.f, 36.f,
            -39.f, 63.f, 92.f,
             18.f, 43.f, 90.f
        };

        for (uint8_t row = 0; row < result.getRows(); ++row) {
            for (uint8_t col = 0; col < result.getColumns(); ++col) {
                ASSERT_NEAR(result(row, col), correctResult(row, col), 0.01f)
                    << "row = " << static_cast<int>(row) << "\n"
                    << "col = " << static_cast<int>(col);
            }
        }

        matrixA *= matrixB;

        for (uint8_t row = 0; row < matrixA.getRows(); ++row) {
            for (uint8_t col = 0; col < matrixA.getColumns(); ++col) {
                ASSERT_NEAR(matrixA(row, col), correctResult(row, col), 0.01f)
                    << "row = " << static_cast<int>(row) << "\n"
                    << "col = " << static_cast<int>(col);
            }
        }
    }

    // Scalar Test
    {
        const lug::Math::Mat3x3f result = matrixB * 2.f;

        const lug::Math::Mat3x3f correctResult{
            1.f * 2.f,  5.f * 2.f, 4.f * 2.f,
            -7.f * 2.f,  3.f * 2.f, 6.f * 2.f,
            5.f * 2.f,  2.f * 2.f, 8.f * 2.f
        };

        for (uint8_t row = 0; row < result.getRows(); ++row) {
            for (uint8_t col = 0; col < result.getColumns(); ++col) {
                ASSERT_NEAR(result(row, col), correctResult(row, col), 0.01f)
                    << "row = " << static_cast<int>(row) << "\n"
                    << "col = " << static_cast<int>(col);
            }
        }
    }
}

TEST(Matrix3x3, Division) {
    lug::Math::Mat3x3f matrixA{
        2, -6, 8,
        7,  8, 2,
        4,  3, 7
    };
    lug::Math::Mat3x3f matrixB{
        1, 5, 4,
        -7, 3, 6,
        5, 2, 8
    };

    // Operator Tests
    {
        const lug::Math::Mat3x3f result = matrixA / matrixB;

        const lug::Math::Mat3x3f correctResult{
            -362.f / 163.f,  96.f / 163.f,   272.f / 163.f,
            357.f / 163.f, -137.f / 163.f, -35.f / 163.f,
            103.f / 326.f, -3.f / 326.f,    118.f / 163.f
        };

        for (uint8_t row = 0; row < result.getRows(); ++row) {
            for (uint8_t col = 0; col < result.getColumns(); ++col) {
                ASSERT_NEAR(result(row, col), correctResult(row, col), 0.01f)
                    << "row = " << static_cast<int>(row) << "\n"
                    << "col = " << static_cast<int>(col);
            }
        }

        matrixA /= matrixB;

        for (uint8_t row = 0; row < matrixA.getRows(); ++row) {
            for (uint8_t col = 0; col < matrixA.getColumns(); ++col) {
                ASSERT_NEAR(matrixA(row, col), correctResult(row, col), 0.01f)
                    << "row = " << static_cast<int>(row) << "\n"
                    << "col = " << static_cast<int>(col);
            }
        }
    }

    // Scalar Test
    {
        const lug::Math::Mat3x3f result = matrixB / 2.0f;

        const lug::Math::Mat3x3f correctResult{
            1.f / 2.f,  5.f / 2.f, 4.f / 2.f,
            -7.f / 2.f,  3.f / 2.f, 6.f / 2.f,
            5.f / 2.f,  2.f / 2.f, 8.f / 2.f
        };

        for (uint8_t row = 0; row < result.getRows(); ++row) {
            for (uint8_t col = 0; col < result.getColumns(); ++col) {
                ASSERT_NEAR(result(row, col), correctResult(row, col), 0.01f)
                    << "row = " << static_cast<int>(row) << "\n"
                    << "col = " << static_cast<int>(col);
            }
        }
    }
}

