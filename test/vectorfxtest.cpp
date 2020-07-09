//
// Created by Wouter Groeneveld on 14/12/18.
//



#include <gtest/gtest.h>
#include <libgba-sprite-engine/vectorfx.h>
#include "tonc_bios_stub.h"

class VectorFxSuite : public ::testing::Test {
protected:
    VectorFx vector;

    virtual void TearDown() {
    }

    virtual void SetUp() {
        vector = VectorFx({120, 80});
    }
};

INLINE float rnd2(float val) {
    return (float) ((std::floor(val * 10) + .5) / 10);
}

void assertVector(VectorFx expected, VectorFx actual) {
    auto str = "[fx: " + actual.to_string() + ", fl: " + actual.to_stringfl() + "] , [fx: " + expected.to_string() + ", fl: " + expected.to_stringfl() + "]";

    // WHY use rounding instead of checking fixed-point itself?
    // because of a lot of rounding errors... -250 (-0.97) is supposed to be -256 (-1)...
    ASSERT_EQ(rnd2(fx2float(actual.x())), rnd2(fx2float(expected.x()))) << "x incorrect: (act, exp) " << str;
    ASSERT_EQ(rnd2(fx2float(actual.y())), rnd2(fx2float(actual.y()))) << "y incorrect: (act, exp) " << str;
    ASSERT_EQ(rnd2(fx2float(actual.z())), rnd2(fx2float(actual.z()))) << "z incorrect: (act, exp) " << str;
}

TEST_F(VectorFxSuite, ToAndFromFixedIntWorks) {
    auto zAxis = VectorFx::fromInt(0, 0, -10);
    ASSERT_EQ(zAxis.y(), 0);
    ASSERT_EQ(zAxis.z(), -2560);
    auto toInt = zAxis.toInt();
    ASSERT_EQ(toInt.y(), 0);
    ASSERT_EQ(toInt.z(), -10);
}

TEST_F(VectorFxSuite, Normalize) {
    // based on Babylon Math debugging examples
    auto zAxis = VectorFx::fromInt(0, 0, -10);
    auto l = zAxis.length();

    auto expected = VectorFx::fromInt(0, 0, -1);
    zAxis.normalize();

    assertVector(expected, zAxis);
}

TEST_F(VectorFxSuite, MinDot) {
    auto zAxis = VectorFx::fromInt(0, 0, -1);
    auto eye = VectorFx::fromInt(0, 0, 10);
    auto result = fx2int(-VectorFx::dot(zAxis, eye));

    ASSERT_EQ(result, 10);
}

TEST_F(VectorFxSuite, Cross_AssertToInt) {
    auto one = VectorFx::fromInt(0, 1, 0);
    auto two = VectorFx::fromInt(0, 0, -1);
    auto result = VectorFx::cross(one, two).toInt();

    ASSERT_EQ(result.x(), -1);
    ASSERT_EQ(result.y(), 0);
    ASSERT_EQ(result.z(), 0);
}

TEST_F(VectorFxSuite, Cross_AssertFixed) {
    auto one = VectorFx::fromInt(0, 1, 0);
    auto two = VectorFx::fromInt(0, 0, -1);
    auto result = VectorFx::cross(one, two);
    auto expected = VectorFx::fromInt(-1, 0, 0);

    ASSERT_EQ(result.x(), expected.x());
    ASSERT_EQ(result.y(), expected.y());
    ASSERT_EQ(result.z(), expected.z());
}

TEST_F(VectorFxSuite, AddTwoVectors) {
    VectorFx one(1, 2, 3);
    VectorFx two(4, 5, 6);
    auto result = one + two;

    ASSERT_EQ(result.x(), 5);
    ASSERT_EQ(result.y(), 7);
    ASSERT_EQ(result.z(), 9);
}

TEST_F(VectorFxSuite, SubstractTwoVectors) {
    VectorFx one(1, 2, 3);
    VectorFx two(4, 5, 6);
    auto result = one - two;

    ASSERT_EQ(result.x(), -3);
    ASSERT_EQ(result.y(), -3);
    ASSERT_EQ(result.z(), -3);
}

TEST_F(VectorFxSuite, LengthSqrtOfAllPowItself) {
    auto one = VectorFx::fromInt(1, 2, 3); // 1*1 + 2*2 + 3*3 = 14
    auto result = fx2float(one.length());
    ASSERT_EQ(3.735, (std::floor(result * 100) + .5) / 100);

    auto negative = VectorFx::fromInt(0, 0, -10);
    int negResult = fx2int(negative.length());

    ASSERT_EQ(fx2int(fxmul(int2fx(-10), int2fx(-10))), 100);
    ASSERT_EQ(negResult, 10);
}

TEST_F(VectorFxSuite, DivideTwoVectors) {
    auto one = VectorFx::fromInt(1, 2, 3);
    auto two = VectorFx::fromInt(4, 10, 6);
    auto result = (two / one).toInt();

    ASSERT_EQ(result.x(), 4);
    ASSERT_EQ(result.y(), 5);
    ASSERT_EQ(result.z(), 2);
}

TEST_F(VectorFxSuite, MultiplyTwoVectors) {
    auto one = VectorFx::fromInt(1, 2, 3);
    auto two = VectorFx::fromInt(4, 10, 6);
    auto result = (two * one).toInt();

    ASSERT_EQ(result.x(), 4);
    ASSERT_EQ(result.y(), 20);
    ASSERT_EQ(result.z(), 18);
}

TEST_F(VectorFxSuite, ScaleAVector) {
    auto one = VectorFx::fromInt(1, 2, 3);
    auto result = one.scale(2).toInt();

    ASSERT_EQ(result.x(), 2);
    ASSERT_EQ(result.y(), 4);
    ASSERT_EQ(result.z(), 6);
}

// ---- //

TEST_F(VectorFxSuite, ToString) {
    ASSERT_EQ(std::string("(120,80,0)"), vector.to_string());
}

TEST_F(VectorFxSuite, LineBetween_Diagonal_ToTopRightCorner) {
    auto points = vector.bresenhamLineTo({240, 0});
    ASSERT_EQ(121, points.size());
    VECTOR pt;

    // some sampling
    pt = points.at(1);
    ASSERT_EQ(121, pt.x);
    ASSERT_EQ(79, pt.y);
    pt = points.at(2);
    ASSERT_EQ(122, pt.x);
    ASSERT_EQ(79, pt.y);
    pt = points.at(10);
    ASSERT_EQ(130, pt.x);
    ASSERT_EQ(73, pt.y);
    pt = points.at(100);
    ASSERT_EQ(220, pt.x);
    ASSERT_EQ(13, pt.y);
}

TEST_F(VectorFxSuite, LineBetween_Diagonal_ToTopLeftCorner) {
    auto points = vector.bresenhamLineTo({0, 0});
    ASSERT_EQ(121, points.size());
    VECTOR pt;

    // some sampling
    pt = points.at(1);
    ASSERT_EQ(119, pt.x);
    ASSERT_EQ(79, pt.y);
    pt = points.at(2);
    ASSERT_EQ(118, pt.x);
    ASSERT_EQ(79, pt.y);
    pt = points.at(10);
    ASSERT_EQ(110, pt.x);
    ASSERT_EQ(73, pt.y);
    pt = points.at(100);
    ASSERT_EQ(20, pt.x);
    ASSERT_EQ(13, pt.y);
}

TEST_F(VectorFxSuite, LineBetween_Diagonal_ToBottomLeftCorner) {
    auto points = vector.bresenhamLineTo({0, 160});
    ASSERT_EQ(121, points.size());
    VECTOR pt;

    // some sampling
    pt = points.at(1);
    ASSERT_EQ(119, pt.x);
    ASSERT_EQ(81, pt.y);
    pt = points.at(2);
    ASSERT_EQ(118, pt.x);
    ASSERT_EQ(81, pt.y);
    pt = points.at(10);
    ASSERT_EQ(110, pt.x);
    ASSERT_EQ(87, pt.y);
    pt = points.at(100);
    ASSERT_EQ(20, pt.x);
    ASSERT_EQ(147, pt.y);
}

TEST_F(VectorFxSuite, LineBetween_Diagonal_ToBottomRightCorner) {
    auto points = vector.bresenhamLineTo({240, 160});
    ASSERT_EQ(121, points.size());
    VECTOR pt;

    // some sampling
    pt = points.at(1);
    ASSERT_EQ(121, pt.x);
    ASSERT_EQ(81, pt.y);
    pt = points.at(2);
    ASSERT_EQ(122, pt.x);
    ASSERT_EQ(81, pt.y);
    pt = points.at(10);
    ASSERT_EQ(130, pt.x);
    ASSERT_EQ(87, pt.y);
    pt = points.at(100);
    ASSERT_EQ(220, pt.x);
    ASSERT_EQ(147, pt.y);
}

TEST_F(VectorFxSuite, LineBetween_Horizontal_FromCenterToHalfYZeroX) {
    auto points = vector.bresenhamLineTo({0, 80});
    ASSERT_EQ(121, points.size());
    for(int i = 0; i <= 120; i++) {
        auto &vec = points.front();
        ASSERT_EQ(80, vec.y);
        ASSERT_EQ(120 - i, vec.x);
        points.pop_front();
    }
}

TEST_F(VectorFxSuite, LineBetween_Vertical_FromCenterToHalfXFullY) {
    auto points = vector.bresenhamLineTo({120, 160});
    ASSERT_EQ(81, points.size());
    for(int i = 0; i <= (160 - 80); i++) {
        auto &vec = points.front();
        ASSERT_EQ(120, vec.x);
        ASSERT_EQ(80 + i, vec.y);
        points.pop_front();
    }
}
