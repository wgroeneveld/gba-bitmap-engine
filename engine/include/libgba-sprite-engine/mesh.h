//
// Created by Wouter Groeneveld on 08/07/20.
//

#ifndef GBA_BITMAP_ENGINE_PROJECT_MESH_H
#define GBA_BITMAP_ENGINE_PROJECT_MESH_H

#include "gbavector.h"
#include <vector>
#include <memory>

class Mesh {
private:
    GBAVector position;
    GBAVector rotation;

    std::vector<std::unique_ptr<GBAVector>> verticesArr;

public:

    void add(GBAVector v);
    inline std::vector<std::unique_ptr<GBAVector>> const& vertices() const {
        return verticesArr;
    }
    explicit Mesh() {}
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
};

#endif //GBA_BITMAP_ENGINE_PROJECT_MESH_H
