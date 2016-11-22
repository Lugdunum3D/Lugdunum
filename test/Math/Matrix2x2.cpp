#include <gtest/gtest.h>
#include <lug/Math/Matrix.hpp>
#include <lug/Math/Vector.hpp>
#include <Math/MatrixUtils.hpp>

// TODO: Really test all operators and values
// TODO: Test the determinant
// TODO: Test the transpose

TEST(Matrix2x2, Constructors) {
    lug::Math::Mat2x2i matrix{1, 2, 3, 4};

    ASSERT_EQ(matrix(0, 0), 1);
    ASSERT_EQ(matrix(0, 1), 2);
    ASSERT_EQ(matrix(1, 0), 3);
    ASSERT_EQ(matrix(1, 1), 4);
}

TEST(Matrix2x2, GeneralOperators) {
    lug::Math::Mat2x2f matrixL(1.0f);
    lug::Math::Mat2x2f matrixM(1.0f);

    lug::Math::Vec2f vectorU(1.0f);
    lug::Math::Vec2f vectorV(1.0f);

    float x = 1.0f;

    lug::Math::Vec2f vectorA = matrixM * vectorU;
    lug::Math::Vec2f vectorB = vectorV * matrixM;

    lug::Math::Mat2x2f matrixN = x / matrixM;
    lug::Math::Mat2x2f matrixO = matrixM / x;
    lug::Math::Mat2x2f matrixP = x * matrixM;
    lug::Math::Mat2x2f matrixQ = matrixM * x;

    EXPECT_TRUE(!(matrixM != matrixQ));
    EXPECT_TRUE(matrixM == matrixL);
}

TEST(Matrix2x2, InverseOperators) {
    const lug::Math::Mat2x2f goodIdentity = lug::Math::Mat2x2f::identity();

    {
        const lug::Math::Mat2x2f matrix{1, 2, 3, 4};
        const lug::Math::Mat2x2f inverse = matrix.inverse();
        const lug::Math::Mat2x2f identity = matrix * inverse;

        ASSERT_MATRIX_NEAR(2, 2, identity, goodIdentity, 0.01f);
    }

    {
        const lug::Math::Mat2x2f matrix{1, 2, 3, 4};
        const lug::Math::Mat2x2f identity = matrix / matrix;

        ASSERT_MATRIX_NEAR(2, 2, identity, goodIdentity, 0.01f);
    }
}

TEST(Matrix2x2, Size) {
    ASSERT_EQ(lug::Math::Mat2x2f().getRows(), 2);
    ASSERT_EQ(lug::Math::Mat2x2f().getColumns(), 2);
}
