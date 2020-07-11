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

int msecs = 1;
void MonkeyScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager());
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(pal, sizeof(pal)));

    monkey = std::unique_ptr<Mesh>(createMesh());

    engine->getTimer()->start();
}

void MonkeyScene::tick(u16 keys) {
    int curmsecs = engine->getTimer()->getMsecs();
    int elapsed = curmsecs - msecs;
    msecs = curmsecs;
    int fps = 1000 / elapsed;

    TextStream::instance().setText(std::to_string(fps < 0 ? 0 : fps) + std::string(" FPS"), 1, 1);

    monkey->rotate(10, 10);
}
