#include <gtest/gtest.h>
#include <lug/Math/Quaternion.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

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

TEST(Quaternion, Equality) {
    // TODO: Fix equality operators to handle float / double
}

TEST(Quaternion, UnaryMinus) {
    lug::Math::Quatf a{1.0f, 3.0f, 2.0f, 4.0f};
    lug::Math::Quatf b{-1.0f, -3.0f, -2.0f, -4.0f};

    lug::Math::Quatf minusA = -a;
    lug::Math::Quatf minusB = -b;

    QUAT_ASSERT_NEAR(a, minusB, 0.01f);
    QUAT_ASSERT_NEAR(b, minusA, 0.01f);
}

TEST(Quaternion, Conjugate) {
    lug::Math::Quatf a{1.0f, 3.0f, 2.0f, 4.0f};
    lug::Math::Quatf b{1.0f, -3.0f, -2.0f, -4.0f};

    lug::Math::Quatf conjugateA = lug::Math::conjugate(a);
    lug::Math::Quatf conjugateB = lug::Math::conjugate(b);

    QUAT_ASSERT_NEAR(a, conjugateB, 0.01f);
    QUAT_ASSERT_NEAR(b, conjugateA, 0.01f);
}

TEST(Quaternion, Length) {
    lug::Math::Quatf a{2.0f, -2.0f, -8.0f, 3.0f};
    ASSERT_EQ(a.length(), 9.0f);

    lug::Math::Quatf b{2.0f, 5.0f, -4.0f, -2.0f};
    ASSERT_EQ((a * b).length(), a.length() * b.length());
}

TEST(Quaternion, Dot) {
    lug::Math::Quatf a{-1.0f, 2.0f, -3.0f, 4.0f};
    lug::Math::Quatf b{8.0f, 7.0f, 6.0f, 5.0f};

    ASSERT_EQ(lug::Math::dot(a, b), 8.0f);
}

TEST(Quaternion, Base) {
    lug::Math::Quatf one{1.0f, 0.0f, 0.0f, 0.0f};
    lug::Math::Quatf i{0.0f, 1.0f, 0.0f, 0.0f};
    lug::Math::Quatf j{0.0f, 0.0f, 1.0f, 0.0f};
    lug::Math::Quatf k{0.0f, 0.0f, 0.0f, 1.0f};

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
    lug::Math::Quatf a(2.f, -2.f, -8.f, 3.f);
    lug::Math::Quatf b = lug::Math::normalize(a);

    ASSERT_NEAR(b.length(), 1.0, 0.01f);
}

TEST(Quaternion, ToAxisAngle) {
    lug::Math::Quatf q{lug::Math::Geometry::radians(10.0f), {1.0f, 0.0f, 0.0f}};
    lug::Math::Vec3f axis = q.getAxis();
    float angle = q.getAngle();

    ASSERT_NEAR(1.0f, axis.x(), 0.01f);
    ASSERT_NEAR(0.0f, axis.y(), 0.01f);
    ASSERT_NEAR(0.0f, axis.z(), 0.01f);

    ASSERT_NEAR(10.0f, lug::Math::Geometry::degrees(angle), 0.01f);
}
