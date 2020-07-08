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

void WireScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager());
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(pal, sizeof(pal)));

    cube = std::unique_ptr<Mesh>(new Mesh());
    cube->add(GBAVector(1, 1, 1));
    cube->add(GBAVector(10, 10, 1));
    cube->add(GBAVector(20, 20, 1));
    cube->add(GBAVector(30, 30, 1));
    /*
    cube->add(GBAVector(-1, 1, 1));
    cube->add(GBAVector(1, 1, 1));
    cube->add(GBAVector(-1, -1, 1));
    cube->add(GBAVector(-1, -1, -1));
    cube->add(GBAVector(-1, 1, -1));
    cube->add(GBAVector(1, 1, -1));
    cube->add(GBAVector(1, -1, 1));
    cube->add(GBAVector(-1, -1, -1));
     */
}

void WireScene::tick(u16 keys) {

}
