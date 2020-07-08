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

// fixed point math things that were missing from TONC

extern FIXED HALF;
extern FIXED ONE;

// -----
INLINE float gr2rad(uint grad);
INLINE FIXED gr2lut(uint grad);
INLINE FIXED rad2lut(float rad);
INLINE FIXED fxrad2lut(FIXED rad);
INLINE float fx12ToFloat(FIXED fx);
INLINE FIXED fx12Tofx8(FIXED fx12);
INLINE FIXED fxtan(FIXED fxrad);
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
    return fx / (float) (1<<12);
}

INLINE FIXED fx12Tofx8(FIXED fx12) {
    return fx12 >> 4;
}

INLINE FIXED fxtan(FIXED fxrad) {
    FIXED theta = fxrad2lut(fxrad);
    FIXED sin = fx12Tofx8(lu_sin(theta));
    FIXED cos = fx12Tofx8(lu_cos(theta));

    return fxdiv(sin, cos);
}

#endif //GBA_BITMAP_ENGINE_PROJECT_MATH_H
