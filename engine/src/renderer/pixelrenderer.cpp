//
// Created by Wouter Groeneveld on 11/07/20.
//

#include <libgba-bitmap-engine/renderer/renderer.h>
#include <libgba-bitmap-engine/renderer/gba_engine.h>

void PixelRenderer::render(const MatrixFx &transformationMatrix, const MatrixFx &worldView, const Mesh* mesh) {
    for (auto &vertex : mesh->vertices()) {
        auto coords = vertex.get()->coords();
        auto projectedPoint = engine->project(coords, transformationMatrix);
        engine->plotPixel(projectedPoint, mesh->colorIndex());
    }
}
