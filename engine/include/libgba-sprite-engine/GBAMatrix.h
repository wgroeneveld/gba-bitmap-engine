//
// Created by Wouter Groeneveld on 08/07/20.
//

#ifndef GBA_BITMAP_ENGINE_PROJECT_GBAMATRIX_H
#define GBA_BITMAP_ENGINE_PROJECT_GBAMATRIX_H

#define MATRIX_DIMENSION 16

#ifdef CODE_COMPILED_AS_PART_OF_TEST
    #include <libgba-sprite-engine/gba/tonc_math_stub.h>
    #else
    #include <libgba-sprite-engine/gba/tonc_math.h>
#endif

FIXED HALF = float2fx(0.5);
FIXED ONE = int2fx(1);

class GBAMatrix {
private:
    FIXED m[MATRIX_DIMENSION];

public:
    inline static FIXED tan(FIXED angle) {
        FIXED sin = lu_sin(angle) >> 4;
        FIXED cos = lu_cos(angle) >> 4;

        return fxdiv(sin, cos);
    }

    inline static GBAMatrix zero() { return GBAMatrix(); }
    inline FIXED mAt(int index) const {
        return m[index];
    }
    GBAMatrix() {}
    GBAMatrix(FIXED m11, FIXED m12, FIXED m13, FIXED m14, FIXED m21, FIXED m22, FIXED m23, FIXED m24, FIXED m31, FIXED m32, FIXED m33, FIXED m34, FIXED m41, FIXED m42, FIXED m43, FIXED m44) {
        m[0] = m11;
        m[1] = m12;
        m[2] = m13;
        m[3] = m14;

        m[4] = m21;
        m[5] = m22;
        m[6] = m23;
        m[7] = m24;

        m[8] = m31;
        m[9] = m32;
        m[10] = m33;
        m[11] = m34;

        m[12] = m41;
        m[13] = m42;
        m[14] = m43;
        m[15] = m44;
    }

    inline static GBAMatrix perspectiveFovLH(FIXED fov, FIXED aspect, FIXED znear, FIXED zfar) {
        auto matrix = GBAMatrix::zero();
        FIXED tanResult = fxdiv(ONE, tan(fxmul(fov, HALF)));
        matrix.m[0] = fxdiv(tanResult, aspect);
        matrix.m[1] = matrix.m[2] = matrix.m[3] = 0;
        matrix.m[5] = tanResult;
        matrix.m[4] = matrix.m[6] = matrix.m[7] = 0;
        matrix.m[8] = matrix.m[9] = 0;
        matrix.m[10] = fxdiv(-zfar, (znear - zfar));
        matrix.m[11] = ONE;
        matrix.m[12] = matrix.m[13] = matrix.m[15] = 0;
        matrix.m[14] = fxdiv(fxmul(znear, zfar), (znear - zfar));
        return matrix;
    }
};

#endif //GBA_BITMAP_ENGINE_PROJECT_GBAMATRIX_H
