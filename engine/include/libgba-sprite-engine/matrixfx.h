//
// Created by Wouter Groeneveld on 08/07/20.
//

#ifndef GBA_BITMAP_ENGINE_PROJECT_GBAMATRIX_H
#define GBA_BITMAP_ENGINE_PROJECT_GBAMATRIX_H

#define MATRIX_DIMENSION 16
#include <libgba-sprite-engine/math.h>
#include <libgba-sprite-engine/vectorfx.h>

#ifdef CODE_COMPILED_AS_PART_OF_TEST
    #include <libgba-sprite-engine/gba/tonc_math_stub.h>
    #else
    #include <libgba-sprite-engine/gba/tonc_math.h>
#endif

class MatrixFx {
private:
    FIXED m[MATRIX_DIMENSION];

public:

    inline static MatrixFx zero() { return MatrixFx(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); }
    inline static MatrixFx identity() { return MatrixFx(ONE, 0, 0, 0, 0, ONE, 0, 0, 0, 0, ONE, 0, 0, 0, 0, ONE); }
    inline FIXED mAt(int index) const {
        return m[index];
    }
    MatrixFx() {}

    inline static MatrixFx fromFloat(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44) {
        return MatrixFx(
                float2fx(m11),
                float2fx(m12),
                float2fx(m13),
                float2fx(m14),

                float2fx(m21),
                float2fx(m22),
                float2fx(m23),
                float2fx(m24),

                float2fx(m31),
                float2fx(m32),
                float2fx(m33),
                float2fx(m34),

                float2fx(m41),
                float2fx(m42),
                float2fx(m43),
                float2fx(m44)
        );        
    }
    
    inline static MatrixFx fromInt(int m11, int m12, int m13, int m14, int m21, int m22, int m23, int m24, int m31, int m32, int m33, int m34, int m41, int m42, int m43, int m44) {
        return MatrixFx(
                int2fx(m11),
                int2fx(m12),
                int2fx(m13),
                int2fx(m14),

                int2fx(m21),
                int2fx(m22),
                int2fx(m23),
                int2fx(m24),

                int2fx(m31),
                int2fx(m32),
                int2fx(m33),
                int2fx(m34),

                int2fx(m41),
                int2fx(m42),
                int2fx(m43),
                int2fx(m44)
                );
    }
    MatrixFx(FIXED m11, FIXED m12, FIXED m13, FIXED m14, FIXED m21, FIXED m22, FIXED m23, FIXED m24, FIXED m31, FIXED m32, FIXED m33, FIXED m34, FIXED m41, FIXED m42, FIXED m43, FIXED m44) {
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

    inline static VectorFx transformCoordinates(VectorFx vector, MatrixFx transformation) {
        FIXED x = fxmul(vector.x(), transformation.mAt(0)) + fxmul(vector.y(), transformation.mAt(4)) + fxmul(vector.z(), transformation.mAt(8) + transformation.mAt(12));
        FIXED y = fxmul(vector.x(), transformation.mAt(1)) + fxmul(vector.y(), transformation.mAt(5)) + fxmul(vector.z(), transformation.mAt(9) + transformation.mAt(13));
        FIXED z = fxmul(vector.x(), transformation.mAt(2)) + fxmul(vector.y(), transformation.mAt(6)) + fxmul(vector.z(), transformation.mAt(10) + transformation.mAt(14));
        FIXED w = fxmul(vector.x(), transformation.mAt(3)) + fxmul(vector.y(), transformation.mAt(7)) + fxmul(vector.z(), transformation.mAt(11) + transformation.mAt(15));
        return VectorFx(fxdiv(x, w), fxdiv(y, w), fxdiv(z, w));
    }

    inline friend MatrixFx operator*(const MatrixFx &thiz, const MatrixFx &other) {
        auto result = MatrixFx();
        result.m[0] = fxmul(thiz.m[0], other.m[0]) + fxmul(thiz.m[1], other.m[4]) + fxmul(thiz.m[2], other.m[8]) + fxmul(thiz.m[3], other.m[12]);
        result.m[1] = fxmul(thiz.m[0], other.m[1]) + fxmul(thiz.m[1], other.m[5]) + fxmul(thiz.m[2], other.m[9]) + fxmul(thiz.m[3], other.m[13]);
        result.m[2] = fxmul(thiz.m[0], other.m[2]) + fxmul(thiz.m[1], other.m[6]) + fxmul(thiz.m[2], other.m[10]) + fxmul(thiz.m[3], other.m[14]);
        result.m[3] = fxmul(thiz.m[0], other.m[3]) + fxmul(thiz.m[1], other.m[7]) + fxmul(thiz.m[2], other.m[11]) + fxmul(thiz.m[3], other.m[15]);
        result.m[4] = fxmul(thiz.m[4], other.m[0]) + fxmul(thiz.m[5], other.m[4]) + fxmul(thiz.m[6], other.m[8]) + fxmul(thiz.m[7], other.m[12]);
        result.m[5] = fxmul(thiz.m[4], other.m[1]) + fxmul(thiz.m[5], other.m[5]) + fxmul(thiz.m[6], other.m[9]) + fxmul(thiz.m[7], other.m[13]);
        result.m[6] = fxmul(thiz.m[4], other.m[2]) + fxmul(thiz.m[5], other.m[6]) + fxmul(thiz.m[6], other.m[10]) + fxmul(thiz.m[7], other.m[14]);
        result.m[7] = fxmul(thiz.m[4], other.m[3]) + fxmul(thiz.m[5], other.m[7]) + fxmul(thiz.m[6], other.m[11]) + fxmul(thiz.m[7], other.m[15]);
        result.m[8] = fxmul(thiz.m[8], other.m[0]) + fxmul(thiz.m[9], other.m[4]) + fxmul(thiz.m[10], other.m[8]) + fxmul(thiz.m[11], other.m[12]);
        result.m[9] = fxmul(thiz.m[8], other.m[1]) + fxmul(thiz.m[9], other.m[5]) + fxmul(thiz.m[10], other.m[9]) + fxmul(thiz.m[11], other.m[13]);
        result.m[10] = fxmul(thiz.m[8], other.m[2]) + fxmul(thiz.m[9], other.m[6]) + fxmul(thiz.m[10], other.m[10]) + fxmul(thiz.m[11], other.m[14]);
        result.m[11] = fxmul(thiz.m[8], other.m[3]) + fxmul(thiz.m[9], other.m[7]) + fxmul(thiz.m[10], other.m[11]) + fxmul(thiz.m[11], other.m[15]);
        result.m[12] = fxmul(thiz.m[12], other.m[0]) + fxmul(thiz.m[13], other.m[4]) + fxmul(thiz.m[14], other.m[8]) + fxmul(thiz.m[15], other.m[12]);
        result.m[13] = fxmul(thiz.m[12], other.m[1]) + fxmul(thiz.m[13], other.m[5]) + fxmul(thiz.m[14], other.m[9]) + fxmul(thiz.m[15], other.m[13]);
        result.m[14] = fxmul(thiz.m[12], other.m[2]) + fxmul(thiz.m[13], other.m[6]) + fxmul(thiz.m[14], other.m[10]) + fxmul(thiz.m[15], other.m[14]);
        result.m[15] = fxmul(thiz.m[12], other.m[3]) + fxmul(thiz.m[13], other.m[7]) + fxmul(thiz.m[14], other.m[11]) + fxmul(thiz.m[15], other.m[15]);
        return result;
    }

    inline static MatrixFx lookAtLH(VectorFx eye, VectorFx target, VectorFx up) {
        auto zAxis = target - eye;
        zAxis.normalize();
        auto xAxis = VectorFx::cross(up, zAxis);
        xAxis.normalize();
        auto yAxis = VectorFx::cross(zAxis, xAxis);
        yAxis.normalize();
        auto ex = -VectorFx::dot(xAxis, eye);
        auto ey = -VectorFx::dot(yAxis, eye);
        auto ez = -VectorFx::dot(zAxis, eye);

        return MatrixFx(xAxis.x(), yAxis.x(), zAxis.x(), 0, xAxis.y(), yAxis.y(), zAxis.y(), 0, xAxis.z(), yAxis.z(), zAxis.z(), 0, ex, ey, ez, ONE);
    }

    inline static MatrixFx rotationX(FIXED angle) {
        auto result = MatrixFx::zero();
        auto s = fxsin(angle);
        auto c = fxcos(angle);
        result.m[0] = 1.0;
        result.m[15] = 1.0;
        result.m[5] = c;
        result.m[10] = c;
        result.m[9] = -s;
        result.m[6] = s;
        return result;
    }

    inline static MatrixFx rotationY(FIXED angle) {
        auto result = MatrixFx::zero();
        auto s = fxsin(angle);
        auto c = fxcos(angle);
        result.m[5] = 1.0;
        result.m[15] = 1.0;
        result.m[0] = c;
        result.m[2] = -s;
        result.m[8] = s;
        result.m[10] = c;
        return result;
    }

    inline static MatrixFx rotationZ(FIXED angle) {
        auto result = MatrixFx::zero();
        auto s = fxsin(angle);
        auto c = fxcos(angle);
        result.m[10] = 1.0;
        result.m[15] = 1.0;
        result.m[0] = c;
        result.m[1] = s;
        result.m[4] = -s;
        result.m[5] = c;
        return result;
    }

    inline static MatrixFx translation(VectorFx vec) {
        auto result = MatrixFx::identity();
        result.m[12] = vec.x();
        result.m[13] = vec.y();
        result.m[14] = vec.z();
        return result;
    }

    inline static MatrixFx rotationYawPitchRoll(FIXED yaw, FIXED pitch, FIXED roll) {
        return MatrixFx::rotationZ(roll) * MatrixFx::rotationX(pitch) * MatrixFx::rotationY(yaw);
    }

    inline static MatrixFx perspectiveFovLH(FIXED fov, FIXED aspect, FIXED znear, FIXED zfar) {
        auto matrix = MatrixFx::zero();
        FIXED tanResult = fxdiv(ONE, fxtan(fxmul(fov, HALF)));
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
