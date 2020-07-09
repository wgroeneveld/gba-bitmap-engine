//
// Created by Wouter Groeneveld on 08/07/20.
//

#include <gtest/gtest.h>
#include <math.h>
#include <libgba-sprite-engine/matrixfx.h>
#include <libgba-sprite-engine/math.h>

class MatrixFxSuite : public ::testing::Test {
protected:
    virtual void TearDown() {
    }

    virtual void SetUp() {
    }
};

INLINE float rnd2(float val) {
    return (float) ((std::floor(val * 10) + .5) / 10);
}


void assertMatrix(MatrixFx expected, MatrixFx actual) {
    for(int i = 0; i < MATRIX_DIMENSION; i++) {
        auto expect = expected.mAt(i);
        auto act = actual.mAt(i);

        // WHY check the rounded floats instead of fixed numbers? conversion issues. -256 and -257 should be 'equal'
        float expectFl = rnd2(fx2float(expect));
        float actFl = rnd2(fx2float(act));

        ASSERT_EQ(expectFl, actFl)  << "M[" << i << "] does not match: (exp, act) " << expect << ", " << act << " - floats: " << expectFl << ", " << actFl;
    }
}

TEST_F(MatrixFxSuite, lookAtLH_TestData) {
    /*
     * IN:
     * eye 0, 0, 10
     * taget: 0, 0, 0
     * up: 0, 1, 0
     * OUT:
     0: -1
    1: 0
    2: 0
    3: 0
    4: 0
    5: 1
    6: 0
    7: 0
    8: 0
    9: 0
    10: -1
    11: 0
    12: -0
    13: -0
    14: 10
    15: 1
     */
    auto eye = VectorFx::fromInt(0, 0, 10);
    auto target = VectorFx::fromInt(0, 0, 0);
    auto up = VectorFx::up();
    auto result = MatrixFx::lookAtLH(eye, target, up);
    auto expected = MatrixFx(-257, 0, 0, 0, 0, 256, 0, 0, 0, 0, -250, 0, 0, 0, 2500, 256);

    assertMatrix(expected, result);
}


TEST_F(MatrixFxSuite, PerspectiveFovLH_TestData) {
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
    auto result = MatrixFx::perspectiveFovLH(float2fx(0.78), float2fx(1.6), float2fx(0.01), float2fx(1));
    auto expected = MatrixFx::fromFloat(1.565f, 0, 0, 0, 0, 2.505f, 0, 0, 0, 0, 1.005f, 1, 0, 0, -0.005f, 0);

    assertMatrix(expected, result);
}
