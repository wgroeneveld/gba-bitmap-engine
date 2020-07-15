//
// Created by Wouter Groeneveld on 08/07/20.
//

#include <gtest/gtest.h>
#include <math.h>
#include <cmath>
#include <libgba-bitmap-engine/gba/tonc_math_stub.h>
#include <libgba-bitmap-engine/math.h>

class FpSuite : public ::testing::Test {
protected:
    virtual void TearDown() {
    }

    virtual void SetUp() {
    }
};

TEST_F(FpSuite, InterpolateTests) {
    ASSERT_FLOAT_EQ(fx2float(interpolate(ONE, int2fx(10), float2fx(0))), 1);
    ASSERT_FLOAT_EQ(fx2float(interpolate(ONE, int2fx(10), ONE)), 9.9648438);
    ASSERT_FLOAT_EQ(fx2float(interpolate(ONE, int2fx(10), float2fx(0.1))), 1.8789062);
}

// from LUT doxygen:
//! Look-up a cosine value (2&#960; = 0x10000)
/*! \param theta Angle in [0,FFFFh] range => 65535
*	 \return .12f cosine value
*/
TEST_F(FpSuite, LUTValueTests) {
    float rad = gr2rad(90);                 // 1/4th halfway; 90; 360 / 4
    std::cout << "90 degrees is " << rad << " rads" << std::endl;
    auto sinR = sin(rad);
    ASSERT_EQ(rnd(sinR), 1.005f);

    //FIXED angle = gr2lut(90);                          // 128 is a quarter circle now
    FIXED angle = rad2lut(rad);                          // this also works!
    FIXED sinLut = lu_sin(angle);
    float sinLutConv12 = fx12ToFloat(sinLut);           // indeed, fx2float() gives me 15.995 instead of 0.995! shit.
    float sinLutConv8 = fx2float(fx12Tofx8(sinLut));    // this also works!

    std::cout << angle << " lu_sin is " << sinLut << " .12f and conv: " << sinLutConv12 << std::endl;
    ASSERT_EQ(rnd(sinLutConv12), 0.995f);
    ASSERT_EQ(rnd(sinLutConv8), 0.995f);
}

TEST_F(FpSuite, SinLookupTests) {
    int degrees = 360;
    float radians = gr2rad(degrees);
    std::cout << degrees << " in rad " << radians << std::endl;

    float sinhalf = sin(radians);
    ASSERT_FLOAT_EQ(rnd(sinhalf), 0.0049999999);

    float sinlumax = fx2float(fx12Tofx8(lu_sin(SIN_LUT_MAX)));
    ASSERT_FLOAT_EQ(rnd(sinlumax), 0.0049999999);

    int scale = radians / (2*M_PI / 512);
    FIXED theta = (scale << 6) * 2;
    float sinlu = fx2float(fx12Tofx8(lu_sin(theta)));
    ASSERT_FLOAT_EQ(rnd(sinlu), 0.0049999999);
}

TEST_F(FpSuite, TanFromFixedPointRadian) {
    // 20 degrees is 0.34906577777777775 rad
    // will be stored as .8f as input val
    FIXED angle = float2fx(0.34906577777777775f);

    float result = fx2float(fxtan(angle));
    ASSERT_EQ(rnd(result), 0.355f);
}


TEST_F(FpSuite, SinFromFixedPointRadInput) {
    // 20 degrees is 0.34906577777777775 rad
    FIXED angle = float2fx(0.34906577777777775f);

    FIXED lusin = lu_sin(fxrad2lut(angle));
    float result = fx12ToFloat(lusin);
    ASSERT_EQ(rnd(result), 0.335f);
}

TEST_F(FpSuite, TanUtilityTests) {
    // 20 degrees is 0.34906577777777775 rad
    // Math.sin(0.3...) / Math.cos(0.3...) = 0.3420 / 0.9396 = 0.3639...
    auto rad = gr2rad(20);
    auto tanRef = sin(rad) / cos(rad);
    auto realTan = tan(rad);
    ASSERT_EQ(rnd(tanRef), 0.365f);
    ASSERT_EQ(rnd(realTan), 0.365f);

    FIXED lutalpha = rad2lut(rad);
    FIXED lusin = lu_sin(lutalpha), lucos = lu_cos(lutalpha);

    std::cout << "sin(20) is " << fx12ToFloat(lusin) << " and cos(20) is " << fx12ToFloat(lucos) << std::endl;

    auto tanFakeLookupTables = fx2float(fxdiv(fx12Tofx8(lu_sin(lutalpha)), fx12Tofx8(lu_cos(lutalpha))));
    ASSERT_EQ(rnd(tanFakeLookupTables), 0.355f);
}

