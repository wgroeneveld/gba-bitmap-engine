//
// Created by Wouter Groeneveld on 14/12/18.
// This is a FIXED-POINT VECTOR REPRESENTATION!
// Wrapper around Tonc's VECTOR for convenience and readability.
//

#ifndef GBA_VECTOR_H_
#define GBA_VECTOR_H_

#include <string>
#include <deque>
#include <libgba-sprite-engine/gba/tonc_bios.h>

#ifdef CODE_COMPILED_AS_PART_OF_TEST
    #include <libgba-sprite-engine/gba/tonc_math_stub.h>
#else
    #include <libgba-sprite-engine/gba/tonc_math.h>
#endif


class VectorFx {
private:
    VECTOR v;
public:
    VectorFx() : v({}) {}
    VectorFx(const VectorFx& other) : v(other.v) {}
    VectorFx(VECTOR v) : v(v) {}
    VectorFx(FIXED x, FIXED y, FIXED z) : v({ x, y, z}) {}

    static VectorFx fromInt(int x, int y, int z) {
        return VectorFx(int2fx(x), int2fx(y), int2fx(z));
    }

    inline VectorFx toInt() {
        return VectorFx(fx2int(v.x), fx2int(v.y), fx2int(v.z));
    }

    std::deque<VECTOR> bresenhamLineTo(VECTOR dest);

    // WHY all these inlines? performance reasons.
    inline static VectorFx up() { return VectorFx::fromInt(0, 1, 0); }
    inline static FIXED dot(const VectorFx &left, const VectorFx &right) {
        return (fxmul(left.v.x, right.v.x) + fxmul(left.v.y, right.v.y) + fxmul(left.v.z, right.v.z));
    }
    inline static VectorFx cross(const VectorFx &left, const VectorFx &right) {
        FIXED x = fxmul(left.v.y, right.v.z) - fxmul(left.v.z, right.v.y);
        FIXED y = fxmul(left.v.z, right.v.x) - fxmul(left.v.x, right.v.z);
        FIXED z = fxmul(left.v.x, right.v.y) - fxmul(left.v.y, right.v.x);
        return VectorFx(x, y, z);
    }

    inline friend VectorFx operator+(const VectorFx &one, const VectorFx &two) {
        return VectorFx(one.v.x + two.v.x, one.v.y + two.v.y, one.v.z + two.v.z);
    }
    inline friend VectorFx operator-(const VectorFx &one, const VectorFx &two) {
        return VectorFx(one.v.x - two.v.x, one.v.y - two.v.y, one.v.z - two.v.z);
    }
    inline friend VectorFx operator*(const VectorFx &one, const VectorFx &two) {
        return VectorFx(fxmul(one.v.x, two.v.x), fxmul(one.v.y, two.v.y), fxmul(one.v.z, two.v.z));
    }
    inline friend VectorFx operator/(const VectorFx &one, const VectorFx &two) {
        return VectorFx(fxdiv(one.v.x, two.v.x), fxdiv(one.v.y, two.v.y), fxdiv(one.v.z, two.v.z));
    }
    inline FIXED length() {
        FIXED toRoot = fxmul(v.x, v.x) + fxmul(v.y, v.y) + fxmul(v.z, v.z);
        // tonc's fx() methods are .8fx, and BIOS needs .16 (results are .8??)
        return Sqrt(toRoot << 8);
    }
    inline VectorFx negate() {
        return VectorFx(-v.x, -v.y, -v.z);
    }
    inline VectorFx scale(int scale) {
        FIXED fac = int2fx(scale);
        return VectorFx(fxmul(v.x, fac), fxmul(v.y, fac), fxmul(v.z, fac));
    }
    inline void normalize() {
        auto len = length();
        if(len == 0) return;
        FIXED num = fxdiv(int2fx(1), len);
        v.x = fxmul(v.x, num);
        v.y = fxmul(v.y, num);
        v.z = fxmul(v.z, num);
    }

    inline FIXED x() const { return v.x; }
    inline float floatX() const { return fx2float(v.x); }
    inline void setX(FIXED x) { v.x = x; }

    inline FIXED y() const { return v.y; }
    inline float floatY() const { return fx2float(v.y); }
    inline void setY(FIXED y) { v.y = y; }

    inline FIXED z() const { return v.z; }
    inline float floatZ() const { return fx2float(v.z); }
    inline void setZ(FIXED z) { v.z = z; }

    std::string to_string() {
        return "(" + std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z) + ")";
    }
    std::string to_stringfl() {
        return "(" + std::to_string(fx2float(v.x)) + "," + std::to_string(fx2float(v.y)) + "," + std::to_string(fx2float(v.z)) + ")";
    }
};

#endif
