//
// Created by Wouter Groeneveld on 02/08/18.
//

#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include "wirescene.h"


const unsigned short pal[3] __attribute__((aligned(4))) = {
        0x0000, 0xFFFF, 0x3AE2
};

void WireScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager());
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(pal, sizeof(pal)));
}

void WireScene::tick(u16 keys) {

}
