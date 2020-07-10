//
// Created by Wouter Groeneveld on 08/07/20.
//

#include <libgba-sprite-engine/mesh.h>
#include <memory>


void Mesh::add(VectorFx v) {
    verticesArr.push_back(std::unique_ptr<VectorFx>(new VectorFx(v)));
}

void Mesh::addFace(Face f) {
    facesArr.push_back(f);
}
