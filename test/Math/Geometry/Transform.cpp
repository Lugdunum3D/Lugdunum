#include <gtest/gtest.h>
#include <lug/Math/Geometry/Transform.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

// TODO: Add tests for lookAt, ortho and perspective

namespace lug {
namespace Math {

TEST(Transform, Translate) {
    const Vec3i translateVector{1, 2, 3};
    const Mat4x4i translateMatrix = Geometry::translate(translateVector);

    Vec4i point{1, 2, 3, 1};

    point *= translateMatrix;

    ASSERT_EQ(point.x(), 2);
    ASSERT_EQ(point.y(), 4);
    ASSERT_EQ(point.z(), 6);
}

TEST(Transform, Rotate) {
    #define TEST_ROTATION(angle, axis_x, axis_y, axis_z, point_x, point_y, point_z, expected_x, expected_y, expected_z, abs_error)  \
    {                                                                                                                               \
        const Vec3d axis{axis_x, axis_y, axis_z};                                                                                   \
        const Mat4x4d rotateMatrix = Geometry::rotate(Geometry::radians(angle), axis);                                              \
                                                                                                                                    \
        Vec4d point{point_x, point_y, point_z, 1.0};                                                                                \
                                                                                                                                    \
        point *= rotateMatrix;                                                                                                      \
                                                                                                                                    \
        ASSERT_NEAR(point.x(), expected_x, abs_error);                                                                              \
        ASSERT_NEAR(point.y(), expected_y, abs_error);                                                                              \
        ASSERT_NEAR(point.z(), expected_z, abs_error);                                                                              \
    }

    #define TEST_SOME_ROTATIONS(point_x, point_y, point_z, abs_error)                                           \
        TEST_ROTATION(0.0  , 1.0, 0.0, 0.0, point_x, point_y, point_z, point_x, point_y, point_z, abs_error)    \
        TEST_ROTATION(90.0 , 1.0, 0.0, 0.0, point_x, point_y, point_z, point_x, point_z, -point_y, abs_error)   \
        TEST_ROTATION(180.0, 1.0, 0.0, 0.0, point_x, point_y, point_z, point_x, -point_y, -point_z, abs_error)  \
        TEST_ROTATION(270.0, 1.0, 0.0, 0.0, point_x, point_y, point_z, point_x, -point_z, point_y, abs_error)   \
        TEST_ROTATION(360.0, 1.0, 0.0, 0.0, point_x, point_y, point_z, point_x, point_y, point_z, abs_error)    \
                                                                                                                \
        TEST_ROTATION(0.0  , 0.0, 1.0, 0.0, point_x, point_y, point_z, point_x, point_y, point_z, abs_error)    \
        TEST_ROTATION(90.0 , 0.0, 1.0, 0.0, point_x, point_y, point_z, -point_z, point_y, point_x, abs_error)   \
        TEST_ROTATION(180.0, 0.0, 1.0, 0.0, point_x, point_y, point_z, -point_x, point_y, -point_z, abs_error)  \
        TEST_ROTATION(270.0, 0.0, 1.0, 0.0, point_x, point_y, point_z, point_z, point_y, -point_x, abs_error)   \
        TEST_ROTATION(360.0, 0.0, 1.0, 0.0, point_x, point_y, point_z, point_x, point_y, point_z, abs_error)    \
                                                                                                                \
        TEST_ROTATION(0.0  , 0.0, 0.0, 1.0, point_x, point_y, point_z, point_x, point_y, point_z, abs_error)    \
        TEST_ROTATION(90.0 , 0.0, 0.0, 1.0, point_x, point_y, point_z, point_y, -point_x, point_z, abs_error)   \
        TEST_ROTATION(180.0, 0.0, 0.0, 1.0, point_x, point_y, point_z, -point_x, -point_y, point_z, abs_error)  \
        TEST_ROTATION(270.0, 0.0, 0.0, 1.0, point_x, point_y, point_z, -point_y, point_x, point_z, abs_error)   \
        TEST_ROTATION(360.0, 0.0, 0.0, 1.0, point_x, point_y, point_z, point_x, point_y, point_z, abs_error)


    TEST_SOME_ROTATIONS(1.0, 2.0, 3.0, 0.01);
}

TEST(Transform, Scale) {
    const Vec3i factors{1, 2, 3};
    const Mat4x4i scaleMatrix = Geometry::scale(factors);

    Vec4i point{1, 2, 3, 1};

    point *= scaleMatrix;

    ASSERT_EQ(point.x(), 1);
    ASSERT_EQ(point.y(), 4);
    ASSERT_EQ(point.z(), 9);
}

} // Math
} // lug
