//
// Created by Wouter Groeneveld on 08/07/20.
//

#include <gtest/gtest.h>
#include <math.h>
#include <libgba-sprite-engine/gbamatrix.h>

class GBAMatrixSuite : public ::testing::Test {
protected:
    virtual void TearDown() {
    }

    virtual void SetUp() {
    }
};

TEST_F(GBAMatrixSuite, TanUtilityTests) {
    // Math.sin(14) / Math.cos(14) = 0.99 / 0.13 = 7.2446
    auto tanRef = sin(14) / cos(14);
    auto realTan = tan(14);
    ASSERT_EQ((std::floor(tanRef * 100) + .5) / 100, 7.245);
    ASSERT_EQ((std::floor(realTan * 100) + .5) / 100, 7.245);

    auto tanFakeLookupTables = ((lu_sin(14)) / (lu_cos(14))) << 4;
    ASSERT_EQ((std::floor(tanFakeLookupTables  * 100) + .5) / 100, 7.245);

    auto result = fx2float(GBAMatrix::tan(int2fx(14)));
    ASSERT_EQ(result, 7.2446);
}

TEST_F(GBAMatrixSuite, PerspectiveFovLH_TestData) {
    /* IN:
     * fov 0.78, aspect 1.6, znear 0.01, zfar 1
     * OUT:
     0: 1.520478108791285
    1: 0
    2: 0
    3: 0
    4: 0
    5: 2.4327649740660564
    6: 0
    7: 0
    8: 0
    9: 0
    10: 1.0101010101010102
    11: 1
    12: 0
    13: 0
    14: -0.010101010101010102
    15: 0
     */
    auto result = GBAMatrix::perspectiveFovLH(float2fx(0.78), float2fx(1.6), float2fx(0.01), float2fx(1));
    ASSERT_EQ(fx2float(result.mAt(0)), 1.52);
}
