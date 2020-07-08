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


class GBAVector {
private:
    VECTOR v;
public:
    GBAVector() : v({}) {}
    GBAVector(const GBAVector& other) : v(other.v) {}
    GBAVector(VECTOR v) : v(v) {}
    GBAVector(FIXED x, FIXED y, FIXED z) : v({ x, y, z}) {}

    static GBAVector fromInt(int x, int y, int z) {
        return GBAVector(int2fx(x), int2fx(y), int2fx(z));
    }

    inline GBAVector toInt() {
        return GBAVector(fx2int(v.x), fx2int(v.y), fx2int(v.z));
    }

    std::deque<VECTOR> bresenhamLineTo(VECTOR dest);
    GBAVector rotateAsCenter(GBAVector point, FIXED angle);

    // WHY all these inlines? performance reasons.
    inline static GBAVector up() { return GBAVector(0, int2fx(1), 0); }
    inline static FIXED dot(const GBAVector &left, const GBAVector &right) {
        return (fxmul(left.v.x, right.v.x) + fxmul(left.v.y, right.v.y) + fxmul(left.v.z, right.v.z));
    }
    inline static GBAVector cross(const GBAVector &left, const GBAVector &right) {
        FIXED x = fxmul(left.v.y, right.v.z) - fxmul(left.v.z, right.v.y);
        FIXED y = fxmul(left.v.z, right.v.x) - fxmul(left.v.x, right.v.z);
        FIXED z = fxmul(left.v.x, right.v.y) - fxmul(left.v.y, right.v.x);
        return GBAVector(x, y, z);
    }

    inline friend GBAVector operator+(const GBAVector &one, const GBAVector &two) {
        return GBAVector(one.v.x + two.v.x, one.v.y + two.v.y, one.v.z + two.v.z);
    }
    inline friend GBAVector operator-(const GBAVector &one, const GBAVector &two) {
        return GBAVector(one.v.x - two.v.x, one.v.y - two.v.y, one.v.z - two.v.z);
    }
    inline friend GBAVector operator*(const GBAVector &one, const GBAVector &two) {
        return GBAVector(fxmul(one.v.x, two.v.x), fxmul(one.v.y, two.v.y), fxmul(one.v.z, two.v.z));
    }
    inline friend GBAVector operator/(const GBAVector &one, const GBAVector &two) {
        return GBAVector(fxdiv(one.v.x, two.v.x), fxdiv(one.v.y, two.v.y), fxdiv(one.v.z, two.v.z));
    }
    inline FIXED length() {
        FIXED toRoot = fxmul(v.x, v.x) + fxmul(v.y, v.y) + fxmul(v.z, v.z);
        return Sqrt(toRoot);
    }
    inline GBAVector negate() {
        return GBAVector(-v.x, -v.y, -v.z);
    }
    inline GBAVector scale(int scale) {
        FIXED fac = int2fx(scale);
        return GBAVector(fxmul(v.x, fac), fxmul(v.y, fac), fxmul(v.z, fac));
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
    inline FIXED y() const { return v.y; }
    inline float floatY() const { return fx2float(v.y); }
    inline FIXED z() const { return v.z; }
    inline float floatZ() const { return fx2float(v.z); }

    std::string to_string() {
        return "(" + std::to_string(v.x) + "," + std::to_string(v.y) + ")";
    }
};

#endif
