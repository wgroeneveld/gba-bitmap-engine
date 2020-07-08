//
// Created by Wouter Groeneveld on 08/07/20.
//

#include <libgba-sprite-engine/mesh.h>
#include <memory>


void Mesh::add(GBAVector v) {
    verticesArr.push_back(std::unique_ptr<GBAVector>(new GBAVector(v)));
}

