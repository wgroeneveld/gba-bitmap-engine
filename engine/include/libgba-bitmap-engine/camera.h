//
// Created by Wouter Groeneveld on 09/07/20.
//

#ifndef GBA_BITMAP_ENGINE_PROJECT_CAMERA_H
#define GBA_BITMAP_ENGINE_PROJECT_CAMERA_H

#include <libgba-bitmap-engine/vectorfx.h>

class Camera {
private:
    VectorFx position;
    VectorFx target;

public:
    Camera() : position(VectorFx()), target(VectorFx()) {}
    Camera(VectorFx pos, VectorFx targ) : position(pos), target(targ) {}

    VectorFx getPosition() { return position; }
    VectorFx getTarget() { return target; }
    void setPosition(VectorFx pos) {
        position = pos;
    }
    void setTarget(VectorFx tar) {
        target = tar;
    }

};

#endif //GBA_BITMAP_ENGINE_PROJECT_CAMERA_H
