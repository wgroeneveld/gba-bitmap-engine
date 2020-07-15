//
// Created by Wouter Groeneveld on 08/07/20.
//

#include <libgba-bitmap-engine/mesh.h>
#include <memory>


void Mesh::add(VectorFx v) {
    verticesArr.push_back(std::unique_ptr<Vertex>(new Vertex(v)));
}

void Mesh::add(VectorFx coords, VectorFx normal) {
    verticesArr.push_back(std::unique_ptr<Vertex>(new Vertex(coords, normal)));
}

void Mesh::addFace(int a, int b, int c) {
    auto &vertexA = verticesArr[a];
    auto &vertexB = verticesArr[b];
    auto &vertexC = verticesArr[c];

    facesArr.push_back(std::unique_ptr<Face>(new Face(a, b, c, *vertexA, *vertexB, *vertexC)));
}
