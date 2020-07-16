//
// Created by Wouter Groeneveld on 15/07/20.
//

#ifndef GBA_BITMAP_ENGINE_PROJECT_VERTEX_H
#define GBA_BITMAP_ENGINE_PROJECT_VERTEX_H

#include <libgba-bitmap-engine/vectorfx.h>

class Vertex {
private:
    VectorFx coordinates;
    VectorFx norm;
    FIXED tU, tV;

public:
    Vertex(const VectorFx& coord) : coordinates(coord), norm(VectorFx()), tU(0), tV(0) {}
    Vertex(const VectorFx& coord, const VectorFx& theNorm) : coordinates(coord), norm(theNorm), tU(0), tV(0) {}
    Vertex(const VectorFx& coord, const VectorFx& theNorm, float theU, float theV) : coordinates(coord), norm(theNorm), tU(float2fx(theU)), tV(float2fx(theV)) {}

    inline const VectorFx& coords() const { return coordinates; }
    inline const VectorFx& normal() const { return norm; }
    inline FIXED u() { return tU; }
    inline FIXED v() { return tV; }
};

#endif //GBA_BITMAP_ENGINE_PROJECT_VERTEX_H
