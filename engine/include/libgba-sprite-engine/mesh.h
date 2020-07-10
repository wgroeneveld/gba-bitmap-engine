//
// Created by Wouter Groeneveld on 08/07/20.
//

#ifndef GBA_BITMAP_ENGINE_PROJECT_MESH_H
#define GBA_BITMAP_ENGINE_PROJECT_MESH_H

#include "vectorfx.h"
#include <vector>
#include <memory>

class Mesh {
private:
    VectorFx pos;
    VectorFx rot;

    std::vector<std::unique_ptr<VectorFx>> verticesArr;

public:

    void add(VectorFx v);
    inline std::vector<std::unique_ptr<VectorFx>> const& vertices() const {
        return verticesArr;
    }

    inline VectorFx &position() { return pos; }
    inline VectorFx &rotation() { return rot; }

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

    explicit Mesh() : pos(VectorFx()), rot(VectorFx()) {}
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
};

#endif //GBA_BITMAP_ENGINE_PROJECT_MESH_H
