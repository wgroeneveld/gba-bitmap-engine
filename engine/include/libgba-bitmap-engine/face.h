//
// Created by Wouter Groeneveld on 15/07/20.
//

#ifndef GBA_BITMAP_ENGINE_PROJECT_FACE_H
#define GBA_BITMAP_ENGINE_PROJECT_FACE_H

#include <libgba-bitmap-engine/vectorfx.h>

class Face {
private:
    int theA, theB, theC;
    VectorFx norm;

public:
    Face(int newA, int newB, int newC, const Vertex& va, const Vertex& vb, const Vertex& vc) : theA(newA), theB(newB), theC(newC), norm(VectorFx()) {
        normalize(va, vb, vc);
    }

    inline const VectorFx& normal() const { return norm; };
    inline int a() { return theA; };
    inline int b() { return theB; };
    inline int c() { return theC; };

    inline void normalize(const Vertex& va, const Vertex& vb, const Vertex& vc) {
        norm = (va.normal() + vb.normal() + vc.normal()).scale(fxdiv(int2fx(1), int2fx(3)));
        norm.normalize();
    }

};

#endif //GBA_BITMAP_ENGINE_PROJECT_FACE_H
