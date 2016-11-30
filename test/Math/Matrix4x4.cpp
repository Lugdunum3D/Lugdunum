#include <gtest/gtest.h>
#include <lug/Math/Matrix.hpp>
#include <lug/Math/Vector.hpp>

// TODO: Really test all operators and values
// TODO: Test the determinant
// TODO: Test the transpose

TEST(Matrix4x4, Constructors) {
    lug::Math::Mat4x4i matrix{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

    ASSERT_EQ(matrix(0, 0), 1);
    ASSERT_EQ(matrix(0, 1), 2);
    ASSERT_EQ(matrix(0, 2), 3);
    ASSERT_EQ(matrix(0, 3), 4);
    ASSERT_EQ(matrix(1, 0), 5);
    ASSERT_EQ(matrix(1, 1), 6);
    ASSERT_EQ(matrix(1, 2), 7);
    ASSERT_EQ(matrix(1, 3), 8);
    ASSERT_EQ(matrix(2, 0), 9);
    ASSERT_EQ(matrix(2, 1), 10);
    ASSERT_EQ(matrix(2, 2), 11);
    ASSERT_EQ(matrix(2, 3), 12);
    ASSERT_EQ(matrix(3, 0), 13);
    ASSERT_EQ(matrix(3, 1), 14);
    ASSERT_EQ(matrix(3, 2), 15);
    ASSERT_EQ(matrix(3, 3), 16);
}

TEST(Matrix4x4, GeneralOperators) {
    lug::Math::Mat4x4f matrixL(1.0f);
    lug::Math::Mat4x4f matrixM(1.0f);

    lug::Math::Vec4f vectorU(1.0f);
    lug::Math::Vec4f vectorV(1.0f);

    float x = 1.0f;

    lug::Math::Vec4f vectorA = matrixM * vectorU;
    lug::Math::Vec4f vectorB = vectorV * matrixM;

    lug::Math::Mat4x4f matrixN = x / matrixM;
    lug::Math::Mat4x4f matrixO = matrixM / x;
    lug::Math::Mat4x4f matrixP = x * matrixM;
    lug::Math::Mat4x4f matrixQ = matrixM * x;

    EXPECT_TRUE(!(matrixM != matrixQ));
    EXPECT_TRUE(matrixM == matrixL);
}

TEST(Matrix4x4, InverseOperators) {
    const lug::Math::Mat4x4d matrix{
        0.6f, 0.2f, 0.3f, 0.4f,
        0.2f, 0.7f, 0.5f, 0.3f,
        0.3f, 0.5f, 0.7f, 0.2f,
        0.4f, 0.3f, 0.2f, 0.6f
    };

    const lug::Math::Mat4x4d goodIdentity = lug::Math::Mat4x4d::identity();

    {
        const lug::Math::Mat4x4d inverse = matrix.inverse();
        const lug::Math::Mat4x4d identity = matrix * inverse;

        for (uint8_t row = 0; row < matrix.getRows(); ++row) {
            for (uint8_t col = 0; col < matrix.getColumns(); ++col) {
                ASSERT_NEAR(identity(row, col), goodIdentity(row, col), 0.01f)
                    << "row = " << static_cast<int>(row) << "\n"
                    << "col = " << static_cast<int>(col);
            }
        }
    }

    {
        const lug::Math::Mat4x4d identity = matrix / matrix;

        for (uint8_t row = 0; row < matrix.getRows(); ++row) {
            for (uint8_t col = 0; col < matrix.getColumns(); ++col) {
                ASSERT_NEAR(identity(row, col), goodIdentity(row, col), 0.01f)
                    << "row = " << static_cast<int>(row) << "\n"
                    << "col = " << static_cast<int>(col);
            }
        }
    }
}

TEST(Matrix4x4, Size) {
    ASSERT_EQ(lug::Math::Mat4x4f().getRows(), 4);
    ASSERT_EQ(lug::Math::Mat4x4f().getColumns(), 4);
}

TEST(Matrix4x4, Addition) {
    const lug::Math::Mat4x4f matrixA{
        2.f, -6.f, 8.f,  3.f,
        7.f,  8.f, 2.f,  4.f,
        4.f,  3.f, 7.f, -8,
        6.f, -4.f, 1.f,  7.f
    };
    const lug::Math::Mat4x4f matrixB{
         1.f,  5.f, 4.f,  4.f,
        -7.f,  3.f, 6.f,  3.f,
         5.f,  2.f, 8.f,  0.f,
         6.f, -1.f, 7.f, -4.f
    };
    const lug::Math::Mat4x4f result = matrixA + matrixB;

    const lug::Math::Mat4x4f correctResult{
        3.f,  -1.f,  12.f,  7.f,
        0.f,   11.f, 8.f,   7.f,
        9.f,   5.f,  15.f, -8.f,
        12.f, -5.f,  8.f,   3.f 
    };

    for (uint8_t row = 0; row < result.getRows(); ++row) {
        for (uint8_t col = 0; col < result.getColumns(); ++col) {
            ASSERT_NEAR(result(row, col), correctResult(row, col), 0.01f)
                << "row = " << static_cast<int>(row) << "\n"
                << "col = " << static_cast<int>(col);
        }
    }
}

TEST(Matrix4x4, Substraction) {
    const lug::Math::Mat4x4f matrixA{
        2.f, -6.f, 8.f,  3.f,
        7.f,  8.f, 2.f,  4.f,
        4.f,  3.f, 7.f, -8,
        6.f, -4.f, 1.f,  7.f
    };
    const lug::Math::Mat4x4f matrixB{
         1.f,  5.f, 4.f,  4.f,
        -7.f,  3.f, 6.f,  3.f,
         5.f,  2.f, 8.f,  0.f,
         6.f, -1.f, 7.f, -4.f
    };
    const lug::Math::Mat4x4f result = matrixA - matrixB;

    const lug::Math::Mat4x4f correctResult{
         1.f, -11.f, 4.f, -1.f,
         14.f, 5.f, -4.f,  1.f,
        -1.f,  1.f, -1.f, -8.f,
         0.f, -3.f, -6.f,  11.f
    };


    for (uint8_t row = 0; row < result.getRows(); ++row) {
        for (uint8_t col = 0; col < result.getColumns(); ++col) {
            ASSERT_NEAR(result(row, col), correctResult(row, col), 0.01f)
                << "row = " << static_cast<int>(row) << "\n"
                << "col = " << static_cast<int>(col);
        }
    }
}

TEST(Matrix4x4, Multiplication) {
    const lug::Math::Mat4x4f matrixA{
        2.f, -6.f, 8.f,  3.f,
        7.f,  8.f, 2.f,  4.f,
        4.f,  3.f, 7.f, -8,
        6.f, -4.f, 1.f,  7.f
    };
    const lug::Math::Mat4x4f matrixB{
         1.f,  5.f, 4.f,  4.f,
        -7.f,  3.f, 6.f,  3.f,
         5.f,  2.f, 8.f,  0.f,
         6.f, -1.f, 7.f, -4.f
    };
    const lug::Math::Mat4x4f result = matrixA * matrixB;

    const lug::Math::Mat4x4f correctResult{
         102.f, 5.f,  57.f, -22.f,
        -15.f,  59.f, 120.f, 36.f,
        -30.f,  51.f, 34.f,  57.f,
         81.f,  13.f, 57.f, -16.f
    };



    for (uint8_t row = 0; row < result.getRows(); ++row) {
        for (uint8_t col = 0; col < result.getColumns(); ++col) {
            ASSERT_NEAR(result(row, col), correctResult(row, col), 0.01f)
                << "row = " << static_cast<int>(row) << "\n"
                << "col = " << static_cast<int>(col);
        }
    }
}

TEST(Matrix4x4, Division) {
    const lug::Math::Mat4x4f matrixA{
        2.f, -6.f, 8.f,  3.f,
        7.f,  8.f, 2.f,  4.f,
        4.f,  3.f, 7.f, -8,
        6.f, -4.f, 1.f,  7.f
    };
    const lug::Math::Mat4x4f matrixB{
         1.f,  5.f, 4.f,  4.f,
        -7.f,  3.f, 6.f,  3.f,
         5.f,  2.f, 8.f,  0.f,
         6.f, -1.f, 7.f, -4.f
    };
    const lug::Math::Mat4x4f result = matrixA / matrixB;

    const lug::Math::Mat4x4f correctResult{
        -3037.f/379.f,  31.f/379.f,   4760.f/379.f, -3298.f/379.f,
         1316.f/379.f, -276.f/379.f, -995.f/379.f,   730.f/379.f,
         2124.f/379.f,  172.f/379.f, -3494.f/379.f,  3011.f/379.f,
        -2912.f/379.f, -365.f/379.f,  5145.f/379.f, -3849.f/379.f
    };

    for (uint8_t row = 0; row < result.getRows(); ++row) {
        for (uint8_t col = 0; col < result.getColumns(); ++col) {
            ASSERT_NEAR(result(row, col), correctResult(row, col), 0.01f)
                << "row = " << static_cast<int>(row) << "\n"
                << "col = " << static_cast<int>(col);
        }
    }
}
