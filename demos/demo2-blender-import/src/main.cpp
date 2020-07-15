//
// Created by Wouter Groeneveld on 11/07/20.
//

#include <libgba-bitmap-engine/scene.h>
#include <libgba-bitmap-engine/renderer/gba_engine.h>
#include <libgba-bitmap-engine/palette/palette_manager.h>

#include "monkey.h"

int main() {
    std::shared_ptr<GBAEngine> engine(new GBAEngine());
    engine.get()->setRenderer(new RasterizerRenderer());

    MonkeyScene* startScene = new MonkeyScene(engine);
    engine->setScene(startScene);

    while (true) {
        engine->update();
    }

    return 0;
}