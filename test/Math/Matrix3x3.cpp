#include <gtest/gtest.h>
#include <lug/Math/Matrix.hpp>
#include <lug/Math/Vector.hpp>

// TODO: Really test all operators and values
// TODO: Test the determinant
// TODO: Test the transpose

TEST(Matrix3x3, Constructors) {
    lug::Math::Mat3x3i matrix{1, 2, 3, 4, 5, 6, 7, 8, 9};

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
