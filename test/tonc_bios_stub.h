//
// Created by Wouter Groeneveld on 08/07/20.
//

#ifndef GBA_BITMAP_ENGINE_PROJECT_TONC_BIOS_STUB_H
#define GBA_BITMAP_ENGINE_PROJECT_TONC_BIOS_STUB_H


namespace externMath {

#include <math.h>

    float root(float num) {
        return sqrt(num);
    }

}

u32 Sqrt(u32 num16fx) {
    // tonc's fx() methods are .8fx, and BIOS needs .16 (results are .8??)
    // this means we expect the input to be .16f
    float numfloat = num16fx / ( (float)( 1<<16	));
    return float2fx(externMath::root(numfloat));
}


#endif //GBA_BITMAP_ENGINE_PROJECT_TONC_BIOS_STUB_H
