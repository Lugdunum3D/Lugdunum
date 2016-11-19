#include <lug/Math/Matrix.hpp>
#include <gtest/gtest.h>

TEST(Matrix, BasicConstruction) {
    lug::Math::Matrix<3, 2, int> matrix{1, 2, 3, 4, 5, 6};

    ASSERT_EQ(matrix(0, 0), 1);
    ASSERT_EQ(matrix(0, 1), 2);
    ASSERT_EQ(matrix(1, 0), 3);
    ASSERT_EQ(matrix(1, 1), 4);
    ASSERT_EQ(matrix(2, 0), 5);
    ASSERT_EQ(matrix(2, 1), 6);
}
