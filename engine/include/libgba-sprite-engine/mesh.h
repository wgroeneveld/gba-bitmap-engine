//
// Created by Wouter Groeneveld on 08/07/20.
//

#ifndef GBA_BITMAP_ENGINE_PROJECT_MESH_H
#define GBA_BITMAP_ENGINE_PROJECT_MESH_H

#include "vectorfx.h"
#include <vector>
#include <memory>

typedef struct {
    int a;
    int b;
    int c;
} Face;

class Mesh {
private:
    VectorFx pos;
    VectorFx rot;
    u8 cIndex;
    bool wired;

    std::vector<std::unique_ptr<VectorFx>> verticesArr;
    std::vector<Face> facesArr;

public:

    void add(VectorFx v);
    void addFace(Face f);
    inline std::vector<std::unique_ptr<VectorFx>> const& vertices() const {
        return verticesArr;
    }

    inline std::vector<Face> const& faces() const {
        return facesArr;
    }

    void setPosition(VectorFx position) { pos = position; }
    inline VectorFx &position() { return pos; }
    inline VectorFx &rotation() { return rot; }

    inline void wire() { wired = true; }
    inline void unwire() { wired = false; }
    inline bool isWired() const { return wired; }
    inline u8 colorIndex() const { return cIndex; }
    inline FIXED rotx() const { return rot.x(); }
    inline FIXED roty() const { return rot.y(); }
    inline FIXED rotz() const { return rot.z(); }

    inline void resetRotation() {
        rot.setX(0);
        rot.setY(0);
    }
    void rotateTo(FIXED x, FIXED y) {
        rot.setX(x);
        rot.setY(y);
    }
    inline void rotate(FIXED x, FIXED y) {
        rot.setX(rot.x() + x);
        rot.setY(rot.y() + y);
    }

    explicit Mesh() : pos(VectorFx()), rot(VectorFx()), cIndex(1), wired(true) {}
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
};

#endif //GBA_BITMAP_ENGINE_PROJECT_MESH_H
