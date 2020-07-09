//
// Created by Wouter Groeneveld on 02/08/18.
//

#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include "wirescene.h"


const unsigned short pal[4] __attribute__((aligned(4))) = {
        0x0000, 0xFFFF, 0x3AE2
};

std::vector<Mesh*> WireScene::meshes() {
    return { cube.get() };
}

Camera WireScene::camera() {
    return Camera(VectorFx::fromInt(0, 0, 10), VectorFx::fromInt(0, 0, 0));
}

void WireScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager());
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(pal, sizeof(pal)));

    cube = std::unique_ptr<Mesh>(new Mesh());
    cube->add(VectorFx::fromInt(-1, 1, 1));
    cube->add(VectorFx::fromInt(1, 1, 1));
    cube->add(VectorFx::fromInt(-1, -1, 1));
    cube->add(VectorFx::fromInt(-1, -1, -1));
    cube->add(VectorFx::fromInt(-1, 1, -1));
    cube->add(VectorFx::fromInt(1, 1, -1));
    cube->add(VectorFx::fromInt(1, -1, 1));
    cube->add(VectorFx::fromInt(1, -1, -1));
    /*
     * should be translated, with camera 0,0,10, and with GBA dimensions, to:
        SoftEngine.js:50 drawing 163,123
        SoftEngine.js:50 drawing 155,115
        SoftEngine.js:50 drawing 155,44
        SoftEngine.js:50 drawing 84,44
        SoftEngine.js:50 drawing 76,123
        SoftEngine.js:50 drawing 84,115
        SoftEngine.js:50 drawing 163,36
        SoftEngine.js:50 drawing 76,36
     */
}

void WireScene::tick(u16 keys) {
    if(keys & KEY_START || keys & KEY_A) {
        cube->rotate(5, 5);
    } else if(keys & KEY_B) {
        cube->resetRotation();
    }
}
