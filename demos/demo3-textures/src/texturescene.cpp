//
// Created by Wouter Groeneveld on 16/07/20.
//

#include "texturescene.h"
#include "texturedata.h"

std::vector<Mesh*> TextureScene::meshes() {
    return { box.get() };
}

Camera TextureScene::camera() {
    return Camera(VectorFx::fromInt(0, 0, 10), VectorFx::fromInt(0, 0, 0));
}

void TextureScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager());
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(sharedPal, sizeof(sharedPal)));

    box = std::unique_ptr<Mesh>(createMesh());
}

void TextureScene::tick(u16 keys) {
    box->rotate(10, 10);
}
