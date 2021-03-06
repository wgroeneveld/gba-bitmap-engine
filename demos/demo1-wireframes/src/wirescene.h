//
// Created by Wouter Groeneveld on 02/08/18.
//

#ifndef GBA_SPRITE_ENGINE_SAMPLE_START_SCENE_H
#define GBA_SPRITE_ENGINE_SAMPLE_START_SCENE_H

#include <libgba-bitmap-engine/scene.h>
#include <libgba-bitmap-engine/mesh.h>
#include <libgba-bitmap-engine/camera.h>
#include <memory>

class WireScene : public Scene {
private:
    std::unique_ptr<Mesh> cube;

public:

    WireScene(std::shared_ptr<GBAEngine> engine) : Scene(engine) {}

    void load() override;
    void tick(u16 keys) override;
    std::vector<Mesh*> meshes() override;
    Camera camera() override;

};


#endif //GBA_SPRITE_ENGINE_SAMPLE_START_SCENE_H
