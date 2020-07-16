//
// Created by Wouter Groeneveld on 16/07/20.
//

#ifndef GBA_BITMAP_ENGINE_PROJECT_TEXTURESCENE_H
#define GBA_BITMAP_ENGINE_PROJECT_TEXTURESCENE_H

#include <libgba-bitmap-engine/scene.h>
#include <libgba-bitmap-engine/mesh.h>
#include <libgba-bitmap-engine/camera.h>
#include <libgba-bitmap-engine/renderer/gba_engine.h>
#include <memory>

// forward declaration of generated mesh.cpp file
Mesh* createMesh();

class TextureScene : public Scene {
    std::unique_ptr<Mesh> box;

public:

    TextureScene(std::shared_ptr<GBAEngine> engine) : Scene(engine) {}

    void load() override;
    void tick(u16 keys) override;
    std::vector<Mesh*> meshes() override;
    Camera camera() override;

};


#endif //GBA_BITMAP_ENGINE_PROJECT_TEXTURESCENE_H
