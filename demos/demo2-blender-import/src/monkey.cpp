//
// Created by Wouter Groeneveld on 11/07/20.
//

#include <libgba-sprite-engine/background/text_stream.h>
#include "monkey.h"

const unsigned short pal[4] __attribute__((aligned(4))) = {
        0x0000, 0xFFFF, 0x3AE2
};

std::vector<Mesh*> MonkeyScene::meshes() {
    return { monkey.get() };
}

Camera MonkeyScene::camera() {
    return Camera(VectorFx::fromInt(0, 0, 10), VectorFx::fromInt(0, 0, 0));
}

void MonkeyScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager());
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(pal, sizeof(pal)));

    monkey = std::unique_ptr<Mesh>(createMesh());
}

void MonkeyScene::tick(u16 keys) {
    monkey->rotate(10, 10);
}
