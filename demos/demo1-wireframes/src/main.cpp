#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/renderer/gba_engine.h>
#include <libgba-sprite-engine/palette/palette_manager.h>

#include "wirescene.h"

int main() {
    std::shared_ptr<GBAEngine> engine(new GBAEngine());
    engine.get()->setRenderer(new WiredRenderer());

    WireScene* startScene = new WireScene(engine);
    engine->setScene(startScene);

    while (true) {
        engine->update();
        engine->delay(1000);
    }

    return 0;
}