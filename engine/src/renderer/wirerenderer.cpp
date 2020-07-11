//
// Created by Wouter Groeneveld on 11/07/20.
//

#include <libgba-sprite-engine/renderer/renderer.h>
#include <libgba-sprite-engine/renderer/gba_engine.h>

void WiredRenderer::render(const MatrixFx &transformationMatrix, const Mesh* mesh) {
    for (auto &face : mesh->faces()) {
        auto &vertexA = mesh->vertices()[face.a];
        auto &vertexB = mesh->vertices()[face.b];
        auto &vertexC = mesh->vertices()[face.c];

        auto pixelA = engine->project(*vertexA.get(), transformationMatrix);
        auto pixelB = engine->project(*vertexB.get(), transformationMatrix);
        auto pixelC = engine->project(*vertexC.get(), transformationMatrix);

        engine->plotLine(pixelA, pixelB, mesh->colorIndex());
        engine->plotLine(pixelB, pixelC, mesh->colorIndex());
        engine->plotLine(pixelC, pixelA, mesh->colorIndex());
    }
}
