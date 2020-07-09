//
// Created by Wouter Groeneveld on 08/07/20.
//

#ifndef GBA_BITMAP_ENGINE_PROJECT_MATH_H
#define GBA_BITMAP_ENGINE_PROJECT_MATH_H

#include <libgba-sprite-engine/gba/tonc_types.h>
#ifdef CODE_COMPILED_AS_PART_OF_TEST
    #include <libgba-sprite-engine/gba/tonc_math_stub.h>
    #else
    #include <libgba-sprite-engine/gba/tonc_math.h>
    #endif

#include <cmath>

// fixed point math things that were missing from TONC

extern FIXED HALF;
extern FIXED ONE;

#define FIX12_SHIFT 12
#define FIX12_SCALE ( 1<<FIX12_SHIFT		)

// -----
INLINE float gr2rad(uint grad);
INLINE FIXED gr2lut(uint grad);
INLINE FIXED rad2lut(float rad);
INLINE FIXED fxrad2lut(FIXED rad);
INLINE float fx12ToFloat(FIXED fx);
INLINE FIXED fx12Tofx8(FIXED fx12);
INLINE FIXED fxtan(FIXED fxrad);
INLINE FIXED fxsin(FIXED fxrad);
INLINE FIXED fxcos(FIXED fxrad);
INLINE float rnd(float val);

// ---- impl
INLINE float rnd(float val) {
    return (float) ((std::floor(val * 100) + .5) / 100);
}

INLINE float gr2rad(uint grad) {
    return grad*M_PI/180;
}

INLINE FIXED gr2lut(uint grad) {
    return 65535 / (360 / grad);
}

INLINE FIXED rad2lut(float rad) {
    return gr2lut(rad * 180 / M_PI);
}

INLINE FIXED fxrad2lut(FIXED rad) {
    // TODO NOT good; too much divisions, but hey, fuck it
    return rad2lut(fx2float(rad));
}

INLINE float fx12ToFloat(FIXED fx) {
    return fx / (float) FIX12_SCALE;
}

INLINE FIXED fx12div(FIXED fa, FIXED fb) {
    return ((fa)*FIX12_SCALE)/(fb);
}

INLINE FIXED fx12Tofx8(FIXED fx12) {
    return fx12 >> 4;
}

INLINE FIXED fxsin(FIXED fxrad) {
    FIXED theta = fxrad2lut(fxrad);
    FIXED sin = lu_sin(theta);
    return fx12Tofx8(sin);
}

INLINE FIXED fxcos(FIXED fxrad) {
    FIXED theta = fxrad2lut(fxrad);
    FIXED cos = lu_cos(theta);
    return fx12Tofx8(cos);
}

INLINE FIXED fxtan(FIXED fxrad) {
    FIXED theta = fxrad2lut(fxrad);
    FIXED sin = lu_sin(theta);
    FIXED cos = lu_cos(theta);

    return fx12Tofx8(fx12div(sin, cos));
}

#endif //GBA_BITMAP_ENGINE_PROJECT_MATH_H
