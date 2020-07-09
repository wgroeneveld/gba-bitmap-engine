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
    inline VectorFx position() { return pos; }
    inline FIXED rotx() { return rot.x(); }
    inline FIXED roty() { return rot.y(); }
    inline FIXED rotz() { return rot.z(); }
    inline void rotate(FIXED x, FIXED y) {
        rot.setX(rot.x() + x);
        rot.setY(rot.y() + y);
    }

    explicit Mesh() {}
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
};

#endif //GBA_BITMAP_ENGINE_PROJECT_MESH_H
