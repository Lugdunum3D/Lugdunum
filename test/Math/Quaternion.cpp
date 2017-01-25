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
    Quatf one{1.0f, 0.0f, 0.0f, 0.0f};
    Quatf i{0.0f, 1.0f, 0.0f, 0.0f};
    Quatf j{0.0f, 0.0f, 1.0f, 0.0f};
    Quatf k{0.0f, 0.0f, 0.0f, 1.0f};

    ASSERT_EQ(one.length(), 1.0f);
    ASSERT_EQ(i.length(), 1.0f);
    ASSERT_EQ(j.length(), 1.0f);
    ASSERT_EQ(k.length(), 1.0f);

    QUAT_ASSERT_NEAR(i * i, -one, 0.01f);
    QUAT_ASSERT_NEAR(j * j, -one, 0.01f);
    QUAT_ASSERT_NEAR(k * k, -one, 0.01f);
    QUAT_ASSERT_NEAR(i * j * k, -one, 0.01f);

    QUAT_ASSERT_NEAR(i * j, k, 0.01f);
    QUAT_ASSERT_NEAR(j * i, -k, 0.01f);
    QUAT_ASSERT_NEAR(j * k, i, 0.01f);
    QUAT_ASSERT_NEAR(k * j, -i, 0.01f);
    QUAT_ASSERT_NEAR(k * i, j, 0.01f);
    QUAT_ASSERT_NEAR(i * k, -j, 0.01f);
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

} // Math
} // lug
