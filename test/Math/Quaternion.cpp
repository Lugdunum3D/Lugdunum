#include <gtest/gtest.h>
#include <lug/Math/Geometry/Trigonometry.hpp>
#include <lug/Math/Quaternion.hpp>

#define QUAT_ASSERT_NEAR(a, b, abs_error)               \
    {                                                   \
        const auto tmpA = a;                            \
        const auto tmpB = b;                            \
                                                        \
        for (uint8_t i = 0; i < 4; ++i) {               \
            ASSERT_NEAR(tmpA[i], tmpB[i], abs_error)    \
                << "i = " << static_cast<int>(i);       \
        }                                               \
    }

namespace lug {
namespace Math {

TEST(Quaternion, Equality) {
    // TODO: Fix equality operators to handle float / double
}

TEST(Quaternion, UnaryMinus) {
    Quatf a{1.0f, 3.0f, 2.0f, 4.0f};
    Quatf b{-1.0f, -3.0f, -2.0f, -4.0f};

    Quatf minusA = -a;
    Quatf minusB = -b;

    QUAT_ASSERT_NEAR(a, minusB, 0.01f);
    QUAT_ASSERT_NEAR(b, minusA, 0.01f);
}

TEST(Quaternion, Conjugate) {
    Quatf a{1.0f, 3.0f, 2.0f, 4.0f};
    Quatf b{1.0f, -3.0f, -2.0f, -4.0f};

    Quatf conjugateA = conjugate(a);
    Quatf conjugateB = conjugate(b);

    QUAT_ASSERT_NEAR(a, conjugateB, 0.01f);
    QUAT_ASSERT_NEAR(b, conjugateA, 0.01f);
}

TEST(Quaternion, Length) {
    Quatf a{2.0f, -2.0f, -8.0f, 3.0f};
    ASSERT_EQ(a.length(), 9.0f);

    Quatf b{2.0f, 5.0f, -4.0f, -2.0f};
    ASSERT_EQ((a * b).length(), a.length() * b.length());
}

TEST(Quaternion, Dot) {
    Quatf a{-1.0f, 2.0f, -3.0f, 4.0f};
    Quatf b{8.0f, 7.0f, 6.0f, 5.0f};

    ASSERT_EQ(dot(a, b), 8.0f);
}

TEST(Quaternion, Base) {
    Quatf quatOne{1.0f, 0.0f, 0.0f, 0.0f};
    Quatf quatI{0.0f, 1.0f, 0.0f, 0.0f};
    Quatf quatJ{0.0f, 0.0f, 1.0f, 0.0f};
    Quatf quatK{0.0f, 0.0f, 0.0f, 1.0f};

    ASSERT_EQ(quatOne.length(), 1.0f);
    ASSERT_EQ(quatI.length(), 1.0f);
    ASSERT_EQ(quatJ.length(), 1.0f);
    ASSERT_EQ(quatK.length(), 1.0f);

    QUAT_ASSERT_NEAR(quatI * quatI, -quatOne, 0.01f);
    QUAT_ASSERT_NEAR(quatJ * quatJ, -quatOne, 0.01f);
    QUAT_ASSERT_NEAR(quatK * quatK, -quatOne, 0.01f);
    QUAT_ASSERT_NEAR(quatI * quatJ * quatK, -quatOne, 0.01f);

    QUAT_ASSERT_NEAR(quatI * quatJ, quatK, 0.01f);
    QUAT_ASSERT_NEAR(quatJ * quatI, -quatK, 0.01f);
    QUAT_ASSERT_NEAR(quatJ * quatK, quatI, 0.01f);
    QUAT_ASSERT_NEAR(quatK * quatJ, -quatI, 0.01f);
    QUAT_ASSERT_NEAR(quatK * quatI, quatJ, 0.01f);
    QUAT_ASSERT_NEAR(quatI * quatK, -quatJ, 0.01f);
}

TEST(Quaternion, Normalize) {
    Quatf a(2.f, -2.f, -8.f, 3.f);
    Quatf b = normalize(a);

    ASSERT_NEAR(b.length(), 1.0, 0.01f);
}

TEST(Quaternion, ToAxisAngle) {
    Quatf q{Geometry::radians(10.0f), {1.0f, 0.0f, 0.0f}};
    Vec3f axis = q.getAxis();
    float angle = q.getAngle();

    ASSERT_NEAR(1.0f, axis.x(), 0.01f);
    ASSERT_NEAR(0.0f, axis.y(), 0.01f);
    ASSERT_NEAR(0.0f, axis.z(), 0.01f);

    ASSERT_NEAR(10.0f, Geometry::degrees(angle), 0.01f);
}

TEST(Quaternion, DirectionTo) {
    Vec3f a{1.0f, 0.0f, 0.0f};
    Vec3f b{-1.0f, 0.0f, 0.0f};
    Vec3f c{1.0f, 1.0f, 0.0f};

    const auto rotAToB = directionTo(a, b);
    a = rotAToB.transform() * a;

    for (uint8_t row = 0; row < a.getRows(); ++row) {
        ASSERT_NEAR(Math::normalize(a)(row), Math::normalize(b)(row), 0.01f)
            << "row = " << static_cast<int>(row);
    }

    const auto rotBToC = directionTo(b, c);
    b = rotBToC.transform() * b;

    for (uint8_t row = 0; row < b.getRows(); ++row) {
        ASSERT_NEAR(Math::normalize(b)(row), Math::normalize(c)(row), 0.01f)
            << "row = " << static_cast<int>(row);
    }
}

} // Math
} // lug
