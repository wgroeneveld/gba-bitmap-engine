//
// Created by Wouter Groeneveld on 11/07/20.
//

#ifndef GBA_BITMAP_ENGINE_PROJECT_MONKEY_H
#define GBA_BITMAP_ENGINE_PROJECT_MONKEY_H

#include <libgba-bitmap-engine/scene.h>
#include <libgba-bitmap-engine/mesh.h>
#include <libgba-bitmap-engine/camera.h>
#include <libgba-bitmap-engine/renderer/gba_engine.h>
#include <memory>

// forward declaration of generated mesh.cpp file
Mesh* createMesh();

class MonkeyScene : public Scene {
private:
    std::unique_ptr<Mesh> monkey;

public:

    MonkeyScene(std::shared_ptr<GBAEngine> engine) : Scene(engine) {}

    void load() override;
    void tick(u16 keys) override;
    std::vector<Mesh*> meshes() override;
    Camera camera() override;


};


#endif //GBA_BITMAP_ENGINE_PROJECT_MONKEY_H
