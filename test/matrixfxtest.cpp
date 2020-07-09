//
// Created by Wouter Groeneveld on 08/07/20.
//

#include <gtest/gtest.h>
#include <math.h>
#include <libgba-sprite-engine/matrixfx.h>
#include <libgba-sprite-engine/math.h>
#include <libgba-sprite-engine/mesh.h>
#include <libgba-sprite-engine/camera.h>
#include <libgba-sprite-engine/gba_engine.h>

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


void assertMatrix(MatrixFx expected, MatrixFx actual, std::string matrixName) {
    for(int i = 0; i < MATRIX_DIMENSION; i++) {
        auto expect = expected.mAt(i);
        auto act = actual.mAt(i);

        // WHY check the rounded floats instead of fixed numbers? conversion issues. -256 and -257 should be 'equal'
        float expectFl = rnd2(fx2float(expect));
        float actFl = rnd2(fx2float(act));

        ASSERT_EQ(expectFl, actFl)  << matrixName << "[" << i << "] does not match: (exp, act) " << expect << ", " << act << " - floats: " << expectFl << ", " << actFl;
    }
}

TEST_F(MatrixFxSuite, RotationMatriches) {
    auto result = MatrixFx::rotationZ(0);
    auto expectedIdMatrix = MatrixFx(float2fx(1.0), 0, 0, 0, 0, float2fx(1.0), 0, 0, 0, 0, float2fx(1.0), 0, 0, 0, 0, float2fx(1.0));
    assertMatrix(expectedIdMatrix, result, "rotz");
}

TEST_F(MatrixFxSuite, RotationYawPitchRoll) {
    auto expectedIdMatrix = MatrixFx(float2fx(1.0), 0, 0, 0, 0, float2fx(1.0), 0, 0, 0, 0, float2fx(1.0), 0, 0, 0, 0, float2fx(1.0));
    auto rotYwaPitchRoll = MatrixFx::rotationYawPitchRoll(0, 0, 0);
    assertMatrix(expectedIdMatrix, rotYwaPitchRoll, "rotywa");
}

TEST_F(MatrixFxSuite, MeshToTransformMatrix_IntegrationTest) {
    // source:
    Mesh cube;
    cube.add(VectorFx(-1, 1, 1));
    cube.add(VectorFx(1, 1, 1));
    cube.add(VectorFx(-1, -1, 1));
    cube.add(VectorFx(-1, -1, -1));
    cube.add(VectorFx(-1, 1, -1));
    cube.add(VectorFx(1, 1, -1));
    cube.add(VectorFx(1, -1, 1));
    cube.add(VectorFx(-1, -1, -1));

    auto currentCamera = Camera(VectorFx::fromInt(0, 0, 10), VectorFx::fromInt(0, 0, 0));
    auto viewMatrix = MatrixFx::lookAtLH(currentCamera.getPosition(), currentCamera.getTarget(), VectorFx::up());

    auto projectionMatrix = MatrixFx::perspectiveFovLH(float2fx(0.78), fxdiv(GBA_SCREEN_WIDTH_FX, GBA_SCREEN_HEIGHT_FX), float2fx(0.01), ONE);
    auto expectedProjectionMatrix = MatrixFx(float2fx(1.64f), 0, 0, 0, 0, float2fx(2.55f), 0, 0, 0, 0, float2fx(1.05), float2fx(1.05), 0, 0, float2fx(-0.05), 0);
    assertMatrix(expectedProjectionMatrix, projectionMatrix, "project");

    auto expectedIdMatrix = MatrixFx(float2fx(1.0), 0, 0, 0, 0, float2fx(1.0), 0, 0, 0, 0, float2fx(1.0), 0, 0, 0, 0, float2fx(1.0));
    auto rotYwaPitchRoll = MatrixFx::rotationYawPitchRoll(cube.roty(), cube.rotx(), cube.rotz());
    assertMatrix(expectedIdMatrix, rotYwaPitchRoll, "rotywa");
    auto translatedPos = MatrixFx::translation(cube.position());
    assertMatrix(expectedIdMatrix, translatedPos, "translpos");

    auto worldMatrix = rotYwaPitchRoll * translatedPos;
    assertMatrix(expectedIdMatrix, worldMatrix, "worldmatrix");

    auto transformMatrix = worldMatrix * viewMatrix * projectionMatrix;
    auto expectedTransformMatrix = MatrixFx(float2fx(-1.67), 0, 0, 0, 0, float2fx(2.55), 0, 0, 0, 0, float2fx(-1.0), float2fx(-1.0), 0, 0, float2fx(9.85), float2fx(9.75));
    assertMatrix(expectedTransformMatrix, transformMatrix, "transfomatrix");

    auto coord = *cube.vertices()[0].get();
    auto point = MatrixFx::transformCoordinates(coord, transformMatrix);
    ASSERT_EQ(fx2float(point.x()), 0.125f);
    ASSERT_EQ(fx2float(point.y()), 0.25f);
    ASSERT_EQ(fx2float(point.z()), 1.00f);

    auto x = fxmul(point.x(), GBA_SCREEN_WIDTH_FX) + fxdiv(GBA_SCREEN_WIDTH_FX, int2fx(2));
    auto y = fxmul(-point.y(), GBA_SCREEN_HEIGHT_FX) + fxdiv(GBA_SCREEN_HEIGHT_FX, int2fx(2));
    ASSERT_EQ(fx2float(x), 150);
    ASSERT_EQ(fx2float(y), 40);

    // dest in Babylon - dest according to for loop below (should print something roughly similar)
    /*
     *  163, 36         - 150,40
     *  76, 36          - 60,40
     *  163, 123        - 150,140
     *  155, 115        - 93,26
     *  155, 44         - 93,115
     *  84, 44          - 172,115
     *  76, 123         - 60,140
     *  84, 115         - 93,26
     *
     */
    for(auto& vertex : cube.vertices()) {
        auto point = MatrixFx::transformCoordinates(*vertex.get(), transformMatrix);
        auto x = fxmul(point.x(), GBA_SCREEN_WIDTH_FX) + fxdiv(GBA_SCREEN_WIDTH_FX, int2fx(2));
        auto y = fxmul(-point.y(), GBA_SCREEN_HEIGHT_FX) + fxdiv(GBA_SCREEN_HEIGHT_FX, int2fx(2));
        std::cout << "plotting (" << fx2int(x) << "," << fx2int(y) << ")" << std::endl;
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

    assertMatrix(expected, result, "M");
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

    assertMatrix(expected, result, "M");
}
