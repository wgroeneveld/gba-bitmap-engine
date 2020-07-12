//
// Created by Wouter Groeneveld on 11/07/20.
//

#include <libgba-sprite-engine/renderer/renderer.h>
#include <libgba-sprite-engine/renderer/gba_engine.h>
#include <libgba-sprite-engine/math.h>
#include <libgba-sprite-engine/background/text_stream.h>

// drawing line between 2 points from left to right
// papb -> pcpd
// pa, pb, pc, pd must then be sorted before
void RasterizerRenderer::processScanLine(FIXED y, const VectorFx& pa, const VectorFx& pb, const VectorFx& pc, const VectorFx& pd, COLOR color) {
    FIXED gradient1 = VectorFx::gradient(y, pa, pb), gradient2 = VectorFx::gradient(y, pc, pd);

    int sx = fx2int(interpolate(pa.x(), pb.x(), gradient1));
    int ex = fx2int(interpolate(pc.x(), pd.x(), gradient2));
    int yy = fx2int(y);

    engine.get()->plotLine(sx, yy, ex, yy, color);
}

void RasterizerRenderer::plotTriangle(const VectorFx& pt1, const VectorFx& pt2, const VectorFx& pt3, COLOR color) {
    // Sorting the points in order to always have this order on screen p1, p2 & p3
    // with p1 always up (thus having the Y the lowest possible to be near the top screen)
    // then p2 between p1 & p3
    auto p1 = &pt1, p2 = &pt2, p3 = &pt3;

    if (p1->y() > p2->y()) {
        auto temp = p2;
        p2 = p1;
        p1 = temp;
    }

    if (p2->y() > p3->y()) {
        auto temp = p2;
        p2 = p3;
        p3 = temp;
    }

    if (p1->y() > p2->y()) {
        auto temp = p2;
        p2 = p1;
        p1 = temp;
    }

    FIXED dP1P2 = VectorFx::slope(*p1, *p2), dP1P3 = VectorFx::slope(*p1, *p3);
    if (dP1P2 > dP1P3) {
        for (auto y = p1->y(); y <= p3->y(); y+=ONE) {
            if (y < p2->y()) {
                processScanLine(y, *p1, *p3, *p1, *p2, color);
            } else {
                processScanLine(y, *p1, *p3, *p2, *p3, color);
            }
        }
    } else {
        for (auto y = p1->y(); y <= p3->y(); y+=ONE) {
            if (y < p2->y()) {
                processScanLine(y, *p1, *p2, *p1, *p3, color);
            } else {
                processScanLine(y, *p2, *p3, *p1, *p3, color);
            }
        }
    }
}

void RasterizerRenderer::render(const MatrixFx &transformationMatrix, const Mesh *mesh) {
    bool colorSwitch = false;
    int i = 0;
    for (auto &face : mesh->faces()) {
        auto &vertexA = mesh->vertices()[face.a];
        auto &vertexB = mesh->vertices()[face.b];
        auto &vertexC = mesh->vertices()[face.c];

        auto pixelA = engine->project(*vertexA.get(), transformationMatrix);
        auto pixelB = engine->project(*vertexB.get(), transformationMatrix);
        auto pixelC = engine->project(*vertexC.get(), transformationMatrix);

        COLOR cI = ONE + fxmul(fxdiv(int2fx(i), int2fx(mesh->faces().size())), int2fx(250));
        plotTriangle(pixelA, pixelB, pixelC, fx2int(cI));
        colorSwitch = !colorSwitch;
        i++;
    }
}