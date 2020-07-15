//
// Created by Wouter Groeneveld on 11/07/20.
//

#ifndef GBA_BITMAP_ENGINE_PROJECT_RENDERER_H
#define GBA_BITMAP_ENGINE_PROJECT_RENDERER_H

#include <libgba-bitmap-engine/matrixfx.h>
#include <libgba-bitmap-engine/mesh.h>

class GBAEngine;

class Renderer {
protected:
    std::shared_ptr<GBAEngine> engine;

public:
    virtual void render(const MatrixFx &transformationMatrix, const Mesh* mesh) = 0;
};

class PixelRenderer : public Renderer {
public:
    void render(const MatrixFx &transformationMatrix, const Mesh* mesh) override;
};

class WiredRenderer : public Renderer {
public:
    void render(const MatrixFx &transformationMatrix, const Mesh* mesh) override;
};

class RasterizerRenderer : public Renderer {
private:
    void plotTriangle(const VectorFx& p1, const VectorFx& p2, const VectorFx& p3, COLOR color);
    void processScanLine(FIXED y, const VectorFx& pa, const VectorFx& pb, const VectorFx& pc, const VectorFx& pd, COLOR color);

public:
    void render(const MatrixFx &transformationMatrix, const Mesh* mesh) override;
};

#endif //GBA_BITMAP_ENGINE_PROJECT_RENDERER_H
