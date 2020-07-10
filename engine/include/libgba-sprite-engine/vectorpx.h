//
// Created by Wouter Groeneveld on 10/07/20.
//

#ifndef GBA_BITMAP_ENGINE_PROJECT_VECTOR2_H
#define GBA_BITMAP_ENGINE_PROJECT_VECTOR2_H

// a 2D vector with pixel-based ints

class VectorPx {
private:
    int v_x, v_y;

public:
    VectorPx(int theX, int theY) : v_x(theX), v_y(theY) {}

    static inline VectorPx fromFx(FIXED x, FIXED y) { return VectorPx(fx2int(x), fx2int(y)); }

    inline int x() const { return v_x; }
    inline int y() const { return v_y; }
};

#endif //GBA_BITMAP_ENGINE_PROJECT_VECTOR2_H
