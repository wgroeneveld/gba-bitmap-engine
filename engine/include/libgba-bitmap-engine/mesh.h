//
// Created by Wouter Groeneveld on 08/07/20.
//

#ifndef GBA_BITMAP_ENGINE_PROJECT_MESH_H
#define GBA_BITMAP_ENGINE_PROJECT_MESH_H

#include <libgba-bitmap-engine/vertex.h>
#include <libgba-bitmap-engine/vectorfx.h>
#include <libgba-bitmap-engine/face.h>
#include <vector>
#include <memory>

class Mesh {
private:
    VectorFx pos;
    VectorFx rot;
    u8 cIndex;

    std::vector<std::unique_ptr<Vertex>> verticesArr;
    std::vector<std::unique_ptr<Face>> facesArr;

public:

    void add(VectorFx coords, VectorFx normal);
    void add(VectorFx coords, VectorFx normal, float u, float v);
    void add(VectorFx v);
    void addFace(int a, int b, int c);

    inline std::vector<std::unique_ptr<Vertex>> const& vertices() const {
        return verticesArr;
    }

    inline std::vector<std::unique_ptr<Face>> const& faces() const {
        return facesArr;
    }

    void setPosition(VectorFx position) { pos = position; }
    inline VectorFx &position() { return pos; }
    inline VectorFx &rotation() { return rot; }

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

    explicit Mesh() : pos(VectorFx()), rot(VectorFx()), cIndex(1) {}
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
};

#endif //GBA_BITMAP_ENGINE_PROJECT_MESH_H
