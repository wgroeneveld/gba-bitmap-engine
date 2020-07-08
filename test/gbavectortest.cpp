//
// Created by Wouter Groeneveld on 14/12/18.
//



#include <gtest/gtest.h>
#include <libgba-sprite-engine/gbavector.h>
#include "tonc_bios_stub.h"

class GBAVectorSuite : public ::testing::Test {
protected:
    GBAVector vector;

    virtual void TearDown() {
    }

    virtual void SetUp() {
        vector = GBAVector({120, 80});
    }
};

TEST_F(GBAVectorSuite, ToAndFromFixedIntWorks) {
    auto zAxis = GBAVector::fromInt(0, 0, -10);
    ASSERT_EQ(zAxis.y(), 0);
    ASSERT_EQ(zAxis.z(), -2560);
    auto toInt = zAxis.toInt();
    ASSERT_EQ(toInt.y(), 0);
    ASSERT_EQ(toInt.z(), -10);
}

TEST_F(GBAVectorSuite, Normalize) {
    // based on Babylon Math debugging examples
    auto zAxis = GBAVector::fromInt(0, 0, -10);
    zAxis.normalize();
    float z = fx2float(zAxis.z());
    auto result = zAxis.toInt();

    ASSERT_EQ(result.x(), 0);
    ASSERT_EQ(result.y(), 0);
    ASSERT_EQ((std::floor(z * 100) + .5) / 100, -0.975);
    //ASSERT_EQ(result.z(), -1);
    // TODO will be rounded to zero. problematic later on?
}

TEST_F(GBAVectorSuite, MinDot) {
    auto zAxis = GBAVector::fromInt(0, 0, -1);
    auto eye = GBAVector::fromInt(0, 0, 10);
    auto result = fx2int(-GBAVector::dot(zAxis, eye));

    ASSERT_EQ(result, 10);
}

TEST_F(GBAVectorSuite, Cross) {
    auto one = GBAVector::fromInt(0, 1, 0);
    auto two = GBAVector::fromInt(0, 0, -1);
    auto result = GBAVector::cross(one, two).toInt();

    ASSERT_EQ(result.x(), -1);
    ASSERT_EQ(result.y(), 0);
    ASSERT_EQ(result.z(), 0);
}

TEST_F(GBAVectorSuite, AddTwoVectors) {
    GBAVector one(1, 2, 3);
    GBAVector two(4, 5, 6);
    auto result = one + two;

    ASSERT_EQ(result.x(), 5);
    ASSERT_EQ(result.y(), 7);
    ASSERT_EQ(result.z(), 9);
}

TEST_F(GBAVectorSuite, SubstractTwoVectors) {
    GBAVector one(1, 2, 3);
    GBAVector two(4, 5, 6);
    auto result = one - two;

    ASSERT_EQ(result.x(), -3);
    ASSERT_EQ(result.y(), -3);
    ASSERT_EQ(result.z(), -3);
}

TEST_F(GBAVectorSuite, LengthSqrtOfAllPowItself) {
    auto one = GBAVector::fromInt(1, 2, 3); // 1*1 + 2*2 + 3*3 = 14
    auto result = fx2float(one.length());
    ASSERT_EQ(3.735, (std::floor(result * 100) + .5) / 100);

    auto negative = GBAVector::fromInt(0, 0, -10);
    int negResult = fx2int(negative.length());

    ASSERT_EQ(fx2int(fxmul(int2fx(-10), int2fx(-10))), 100);
    ASSERT_EQ(negResult, 10);
}

TEST_F(GBAVectorSuite, DivideTwoVectors) {
    auto one = GBAVector::fromInt(1, 2, 3);
    auto two = GBAVector::fromInt(4, 10, 6);
    auto result = (two / one).toInt();

    ASSERT_EQ(result.x(), 4);
    ASSERT_EQ(result.y(), 5);
    ASSERT_EQ(result.z(), 2);
}

TEST_F(GBAVectorSuite, MultiplyTwoVectors) {
    auto one = GBAVector::fromInt(1, 2, 3);
    auto two = GBAVector::fromInt(4, 10, 6);
    auto result = (two * one).toInt();

    ASSERT_EQ(result.x(), 4);
    ASSERT_EQ(result.y(), 20);
    ASSERT_EQ(result.z(), 18);
}

TEST_F(GBAVectorSuite, ScaleAVector) {
    auto one = GBAVector::fromInt(1, 2, 3);
    auto result = one.scale(2).toInt();

    ASSERT_EQ(result.x(), 2);
    ASSERT_EQ(result.y(), 4);
    ASSERT_EQ(result.z(), 6);
}

uint grad2rad(uint grad) {
    //return float2fx(grad*M_PI/180);
    return fxdiv(fxmul(int2fx(grad), float2fx(M_PI)), int2fx(180));
}

TEST_F(GBAVectorSuite, Rotate_FromBottomHalf_0_Degrees) {
    auto bottomHalf = GBAVector::fromInt(120, 200, 0);
    auto vector = GBAVector::fromInt(120, 80, 0);
    auto result = vector.rotateAsCenter(bottomHalf, grad2rad(0)).toInt();
    ASSERT_EQ(120, result.x());
    ASSERT_EQ(200, result.y());
}

TEST_F(GBAVectorSuite, Rotate_FromBottomHalf_90_Degrees) {
    auto bottomHalf = GBAVector::fromInt(120, 200, 0);
    auto vector = GBAVector::fromInt(120, 80, 0);
    auto result = vector.rotateAsCenter(bottomHalf, grad2rad(90)).toInt();
    ASSERT_EQ(240, result.x());
    ASSERT_EQ(80, result.y());
}

TEST_F(GBAVectorSuite, Rotate_FromBottomHalf_180_Degrees) {
    auto bottomHalf = GBAVector::fromInt(120, 200, 0);
    auto vector = GBAVector::fromInt(120, 80, 0);
    auto result = vector.rotateAsCenter(bottomHalf, grad2rad(180)).toInt();
    ASSERT_EQ(120, result.x());
    ASSERT_EQ(-40, result.y());
}

TEST_F(GBAVectorSuite, Rotate_FromBottomHalf_270_Degrees) {
    auto bottomHalf = GBAVector::fromInt(120, 200, 0);
    auto vector = GBAVector::fromInt(120, 80, 0);
    auto result = vector.rotateAsCenter(bottomHalf, grad2rad(270)).toInt();
    ASSERT_EQ(0, result.x());
    ASSERT_EQ(80, result.y());
}
// ---- //

TEST_F(GBAVectorSuite, ToString) {
    ASSERT_EQ(std::string("(120,80)"), vector.to_string());
}

TEST_F(GBAVectorSuite, LineBetween_Diagonal_ToTopRightCorner) {
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

TEST_F(GBAVectorSuite, LineBetween_Diagonal_ToTopLeftCorner) {
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

TEST_F(GBAVectorSuite, LineBetween_Diagonal_ToBottomLeftCorner) {
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

TEST_F(GBAVectorSuite, LineBetween_Diagonal_ToBottomRightCorner) {
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

TEST_F(GBAVectorSuite, LineBetween_Horizontal_FromCenterToHalfYZeroX) {
    auto points = vector.bresenhamLineTo({0, 80});
    ASSERT_EQ(121, points.size());
    for(int i = 0; i <= 120; i++) {
        auto &vec = points.front();
        ASSERT_EQ(80, vec.y);
        ASSERT_EQ(120 - i, vec.x);
        points.pop_front();
    }
}

TEST_F(GBAVectorSuite, LineBetween_Vertical_FromCenterToHalfXFullY) {
    auto points = vector.bresenhamLineTo({120, 160});
    ASSERT_EQ(81, points.size());
    for(int i = 0; i <= (160 - 80); i++) {
        auto &vec = points.front();
        ASSERT_EQ(120, vec.x);
        ASSERT_EQ(80 + i, vec.y);
        points.pop_front();
    }
}
