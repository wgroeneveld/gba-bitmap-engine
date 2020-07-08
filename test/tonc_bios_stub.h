//
// Created by Wouter Groeneveld on 08/07/20.
//

#ifndef GBA_BITMAP_ENGINE_PROJECT_TONC_BIOS_STUB_H
#define GBA_BITMAP_ENGINE_PROJECT_TONC_BIOS_STUB_H


namespace externMath {

#include <math.h>

    float root(float num) {
        return sqrt(num);
        return 0;
    }

}

u32 Sqrt(u32 num) {
    return float2fx(externMath::root(fx2float(num)));
}


#endif //GBA_BITMAP_ENGINE_PROJECT_TONC_BIOS_STUB_H
