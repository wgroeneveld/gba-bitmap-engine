//
// Created by Wouter Groeneveld on 02/08/18.
//

#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/background/text_stream.h>
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

int msecs = 1;
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

    cube->addFace({ 0, 1, 2});
    cube->addFace({ 1, 2, 3});
    cube->addFace({ 1, 3, 6});
    cube->addFace({ 1, 5, 6});
    cube->addFace({ 0, 1, 4});
    cube->addFace({ 1, 4, 5});

    cube->addFace({ 2, 3, 7});
    cube->addFace({ 3, 6, 7});
    cube->addFace({ 0, 2, 7});
    cube->addFace({ 0, 4, 7});
    cube->addFace({ 4, 5, 6});
    cube->addFace({ 4, 6, 7});

    engine->getTimer()->start();
}

void WireScene::tick(u16 keys) {
    int curmsecs = engine->getTimer()->getMsecs();
    int elapsed = curmsecs - msecs;
    msecs = curmsecs;

    TextStream::instance().setText(std::to_string(1000 / elapsed) + std::string(" FPS"), 1, 1);

    cube->rotate(2, 2);

    if(keys & KEY_A) {
        cube->resetRotation();
    } else if(keys & KEY_B) {
        if(cube->isWired()) {
            cube->unwire();
        } else {
            cube->wire();
        }
    }
}
