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
    // texture coords here

public:
    Vertex(const VectorFx& coord) : coordinates(coord), norm(VectorFx()) {}
    Vertex(const VectorFx& coord, const VectorFx& theNorm) : coordinates(coord), norm(theNorm) {}

    inline const VectorFx& coords() const { return coordinates; }
    inline const VectorFx& normal() const { return norm; }
};

#endif //GBA_BITMAP_ENGINE_PROJECT_VERTEX_H
