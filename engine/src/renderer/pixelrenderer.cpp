//
// Created by Wouter Groeneveld on 11/07/20.
//

#include <libgba-bitmap-engine/renderer/renderer.h>
#include <libgba-bitmap-engine/renderer/gba_engine.h>

void PixelRenderer::render(const MatrixFx &transformationMatrix, const Mesh* mesh) {
    for (auto &vertex : mesh->vertices()) {
        auto projectedPoint = engine->project(*vertex.get(), transformationMatrix);
        engine->plotPixel(projectedPoint, mesh->colorIndex());
    }
}
