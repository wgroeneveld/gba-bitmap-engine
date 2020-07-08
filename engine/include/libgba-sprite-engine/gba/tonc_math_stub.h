//
// Created by Wouter Groeneveld on 14/12/18.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_TONC_MATH_STUB_H
#define GBA_SPRITE_ENGINE_PROJECT_TONC_MATH_STUB_H

//
//  Mathematical functions
//
//! \file tonc_math.h
//! \author J Vijn
//! \date 20060508 - 20060908
//
// === NOTES ===


#include <cmath>
#include "tonc_types.h"

// --- Doxygen modules ---

/*! \defgroup grpMathBase	Base math
*	\brief Basic math macros and functions like MIN, MAX
*	\ingroup grpMath
*/

/*! \defgroup grpMathFixed	Fixed point math
*	\ingroup grpMath
*/

/*! \defgroup grpMathLut	Look-up tables
*	\brief Tonc's internal look-up tables and related routines.
*	\ingroup grpMath
*/

/*! \defgroup grpMathPoint	Point functions
*	\ingroup grpMath
*/

/*! \defgroup grpMathVector	Vector functions
*	\ingroup grpMath
*/

/*! \defgroup grpMathRect	Rect functions
*	\ingroup grpMath
*/


// --------------------------------------------------------------------
//   GENERAL
// --------------------------------------------------------------------


/*!	\addtogroup grpMathBase	*/
/*! \{	*/

// Also available as inline functions

//! \name core math macros
//\{

#ifndef ABS
//! Get the absolute value of \a x
#define ABS(x)     ( (x)>=0 ? (x) : -(x) )
#endif	// ABS

#ifndef SGN
//! Get the sign of \a x.
#define SGN(x)     ( (x)>=0 ? 1 : -1 )
#define SGN2    SGN
#endif	// SGN

#ifndef SGN3
//! Tri-state sign: -1 for negative, 0 for 0, +1 for positive.
#define SGN3(x)     ( (x)>0 ? 1 : ( (x)<0 ? -1 : 0) )
#endif	// SGN3

#ifndef MAX

//! Get the maximum of \a a and \a b
#define MAX(a, b)  ( ((a) > (b)) ? (a) : (b) )

//! Get the minimum of \a a and \a b
#define MIN(a, b)  ( ((a) < (b)) ? (a) : (b) )
#endif	// MAX

#ifndef SWAP
//! In-place swap.
#define SWAP2(a, b)	do { a=(a)-(b); b=(a)+(b); a=(b)-(a); } while(0)

#define SWAP	SWAP2

//Alternative:
//#define SWAP2(a, b)	( (b) ^= ((a) ^= ((b) ^= (a))) )

//! Swaps \a a and \a b, using \a tmp as a temporary
#define SWAP3(a, b, tmp)	do { (tmp)=(a); (a)=(b); (b)=(tmp); } while(0)
#endif	// SWAP


INLINE int sgn(int x);
INLINE int sgn3(int x);
INLINE int max(int a, int b);
INLINE int min(int a, int b);

//\}


//! \name Boundary response macros
//\{


//! Range check
#define IN_RANGE(x, min, max)	( ((x)>=(min)) && ((x)<(max)) )

//! Truncates \a x to stay in range [\a min, \a max>
/*!	\return Truncated value of \a x.
*	\note	\a max is exclusive!
*/
#define CLAMP(x, min, max)	\
	( (x)>=(max) ? ((max)-1) : ( ((x)<(min)) ? (min) : (x) ) )

//! Reflects \a x at boundaries \a min and \a max
/*! If \a x is outside the range [\a min, \a max>,
*	  it'll be placed inside again with the same distance
*	  to the 'wall', but on the other side. Example for lower
*	  border: y = \a min - (\a x- \a min)  = 2*\a min + \a x.
*	\return	Reflected value of \a x.
*	\note	\a max is exclusive!
*/
#define REFLECT(x, min, max)	\
	( (x)>=(max) ? 2*((max)-1)-(x) : ( ((x)<(min)) ? 2*(min)-(x) : (x) ) )

//! Wraps \a x to stay in range [\a min, \a max>
#define WRAP(x, min, max)	\
	( (x)>=(max) ? (x)+(min)-(max) : ( ((x)<(min)) ? (x)+(max)-(min) : (x) ) )


INLINE BOOL in_range(int x, int min, int max);
INLINE int clamp(int x, int min, int max);
INLINE int reflect(int x, int min, int max);
INLINE int wrap(int x, int min, int max);

//\}

/* \}	*/


// --------------------------------------------------------------------
//   FIXED POINT
// --------------------------------------------------------------------


/*! \addtogroup grpMathFixed	*/
/*! \{	*/

#define FIX_SHIFT       8
#define FIX_SCALE       ( 1<<FIX_SHIFT		)
#define FIX_MASK        ( FIX_SCALE-1		)
#define FIX_SCALEF      ( (float)FIX_SCALE	)
#define FIX_SCALEF_INV	( 1.0/FIX_SCALEF	)

#define FIX_ONE			FIX_SCALE

//! Get the fixed point reciprocal of \a a, in \a fp fractional bits.
/*!
*	\param a	Value to take the reciprocal of.
*	\param fp	Number of fixed point bits
*	\note	The routine does do a division, but the compiler will
*	  optimize it to a single constant ... \e if both \a a and \a fp
*	  are constants!
*	\sa	#FX_RECIMUL
*/
#define FX_RECIPROCAL(a, fp)	( ((1<<(fp))+(a)-1)/(a) )

//! Perform the division \a x/ \a a by reciprocal multiplication
/*! Division is slow, but you can approximate division by a constant
*	by multiplying with its reciprocal: x/a vs x*(1/a). This routine
*	gives the reciprocal of \a a as a fixed point number with \a fp
*	fractional bits.
*	\param a	Value to take the reciprocal of.
*	\param fp	Number of fixed point bits
*	\note	The routine does do a division, but the compiler will
*	  optimize it to a single constant ... \e if both \a a and \a fp
*	  are constants!
*	\note	Rules for safe reciprocal division, using
*	  n = 2<sup>fp</sup> and m = (n+a-1)/a (i.e., rounding up)
*	  \li	Maximum safe numerator \a x:	x < n/(m*a-n)
*	  \li	Minimum n for known \a x:		n > x*(a-1)
*/
#define FX_RECIMUL(x, a, fp)	( ((x)*((1<<(fp))+(a)-1)/(a))>>(fp) )

INLINE FIXED int2fx(int d);
INLINE FIXED float2fx(float f);
INLINE u32 fx2uint(FIXED fx);
INLINE u32 fx2ufrac(FIXED fx);
INLINE int fx2int(FIXED fx);
INLINE float fx2float(FIXED fx);
INLINE FIXED fxadd(FIXED fa, FIXED fb);
INLINE FIXED fxsub(FIXED fa, FIXED fb);
INLINE FIXED fxmul(FIXED fa, FIXED fb);
INLINE FIXED fxdiv(FIXED fa, FIXED fb);

INLINE FIXED fxmul64(FIXED fa, FIXED fb);
INLINE FIXED fxdiv64(FIXED fa, FIXED fb);

/*! \}	*/

// === LUT ============================================================


/*!	\addtogroup grpMathLut	*/
/*! \{	*/

#define DIV_LUT_SIZE 257	// 256 for main lut, 1 extra for lerp

//
// Sine lut; 512 entries, 12 fixeds
//
// EDIT Wouter Groeneveld, 2020-07-08:
// generated by sin lut generator from https://www.coranac.com/tonc/text/fixed.htm
// use original lu_sin/cos functions and values, but don't link with sin_lut.s (ARM ASM not compatible with Gtest CXX)
#define SIN_LUT_SIZE 512
const short sin_lut[SIN_LUT_SIZE]=
        {
                0x0000, 0x0032, 0x0064, 0x0096, 0x00C8, 0x00FB, 0x012D, 0x015F,
                0x0191, 0x01C3, 0x01F5, 0x0227, 0x0259, 0x028A, 0x02BC, 0x02ED,
                0x031F, 0x0350, 0x0381, 0x03B2, 0x03E3, 0x0413, 0x0444, 0x0474,
                0x04A5, 0x04D5, 0x0504, 0x0534, 0x0563, 0x0593, 0x05C2, 0x05F0,
                0x061F, 0x064D, 0x067B, 0x06A9, 0x06D7, 0x0704, 0x0731, 0x075E,
                0x078A, 0x07B7, 0x07E2, 0x080E, 0x0839, 0x0864, 0x088F, 0x08B9,
                0x08E3, 0x090D, 0x0936, 0x095F, 0x0987, 0x09B0, 0x09D7, 0x09FF,
                0x0A26, 0x0A4D, 0x0A73, 0x0A99, 0x0ABE, 0x0AE3, 0x0B08, 0x0B2C,
                0x0B50, 0x0B73, 0x0B96, 0x0BB8, 0x0BDA, 0x0BFC, 0x0C1D, 0x0C3E,
                0x0C5E, 0x0C7D, 0x0C9D, 0x0CBB, 0x0CD9, 0x0CF7, 0x0D14, 0x0D31,
                0x0D4D, 0x0D69, 0x0D84, 0x0D9F, 0x0DB9, 0x0DD2, 0x0DEB, 0x0E04,
                0x0E1C, 0x0E33, 0x0E4A, 0x0E60, 0x0E76, 0x0E8B, 0x0EA0, 0x0EB4,
                0x0EC8, 0x0EDB, 0x0EED, 0x0EFF, 0x0F10, 0x0F21, 0x0F31, 0x0F40,
                0x0F4F, 0x0F5D, 0x0F6B, 0x0F78, 0x0F85, 0x0F91, 0x0F9C, 0x0FA7,
                0x0FB1, 0x0FBA, 0x0FC3, 0x0FCB, 0x0FD3, 0x0FDA, 0x0FE1, 0x0FE7,
                0x0FEC, 0x0FF0, 0x0FF4, 0x0FF8, 0x0FFB, 0x0FFD, 0x0FFE, 0x0FFF,
                0x0FFF, 0x0FFF, 0x0FFE, 0x0FFD, 0x0FFB, 0x0FF8, 0x0FF4, 0x0FF0,
                0x0FEC, 0x0FE7, 0x0FE1, 0x0FDA, 0x0FD3, 0x0FCB, 0x0FC3, 0x0FBA,
                0x0FB1, 0x0FA7, 0x0F9C, 0x0F91, 0x0F85, 0x0F78, 0x0F6B, 0x0F5D,
                0x0F4F, 0x0F40, 0x0F31, 0x0F21, 0x0F10, 0x0EFF, 0x0EED, 0x0EDB,
                0x0EC8, 0x0EB4, 0x0EA0, 0x0E8B, 0x0E76, 0x0E60, 0x0E4A, 0x0E33,
                0x0E1C, 0x0E04, 0x0DEB, 0x0DD2, 0x0DB9, 0x0D9F, 0x0D84, 0x0D69,
                0x0D4D, 0x0D31, 0x0D14, 0x0CF7, 0x0CD9, 0x0CBB, 0x0C9D, 0x0C7D,
                0x0C5E, 0x0C3E, 0x0C1D, 0x0BFC, 0x0BDA, 0x0BB8, 0x0B96, 0x0B73,
                0x0B50, 0x0B2C, 0x0B08, 0x0AE3, 0x0ABE, 0x0A99, 0x0A73, 0x0A4D,
                0x0A26, 0x09FF, 0x09D7, 0x09B0, 0x0987, 0x095F, 0x0936, 0x090D,
                0x08E3, 0x08B9, 0x088F, 0x0864, 0x0839, 0x080E, 0x07E2, 0x07B7,
                0x078A, 0x075E, 0x0731, 0x0704, 0x06D7, 0x06A9, 0x067B, 0x064D,
                0x061F, 0x05F0, 0x05C2, 0x0593, 0x0563, 0x0534, 0x0504, 0x04D5,
                0x04A5, 0x0474, 0x0444, 0x0413, 0x03E3, 0x03B2, 0x0381, 0x0350,
                0x031F, 0x02ED, 0x02BC, 0x028A, 0x0259, 0x0227, 0x01F5, 0x01C3,
                0x0191, 0x015F, 0x012D, 0x00FB, 0x00C8, 0x0096, 0x0064, 0x0032,
                0x0000, 0xFFCE, 0xFF9C, 0xFF6A, 0xFF38, 0xFF05, 0xFED3, 0xFEA1,
                0xFE6F, 0xFE3D, 0xFE0B, 0xFDD9, 0xFDA7, 0xFD76, 0xFD44, 0xFD13,
                0xFCE1, 0xFCB0, 0xFC7F, 0xFC4E, 0xFC1D, 0xFBED, 0xFBBC, 0xFB8C,
                0xFB5B, 0xFB2B, 0xFAFC, 0xFACC, 0xFA9D, 0xFA6D, 0xFA3E, 0xFA10,
                0xF9E1, 0xF9B3, 0xF985, 0xF957, 0xF929, 0xF8FC, 0xF8CF, 0xF8A2,
                0xF876, 0xF849, 0xF81E, 0xF7F2, 0xF7C7, 0xF79C, 0xF771, 0xF747,
                0xF71D, 0xF6F3, 0xF6CA, 0xF6A1, 0xF679, 0xF650, 0xF629, 0xF601,
                0xF5DA, 0xF5B3, 0xF58D, 0xF567, 0xF542, 0xF51D, 0xF4F8, 0xF4D4,
                0xF4B0, 0xF48D, 0xF46A, 0xF448, 0xF426, 0xF404, 0xF3E3, 0xF3C2,
                0xF3A2, 0xF383, 0xF363, 0xF345, 0xF327, 0xF309, 0xF2EC, 0xF2CF,
                0xF2B3, 0xF297, 0xF27C, 0xF261, 0xF247, 0xF22E, 0xF215, 0xF1FC,
                0xF1E4, 0xF1CD, 0xF1B6, 0xF1A0, 0xF18A, 0xF175, 0xF160, 0xF14C,
                0xF138, 0xF125, 0xF113, 0xF101, 0xF0F0, 0xF0DF, 0xF0CF, 0xF0C0,
                0xF0B1, 0xF0A3, 0xF095, 0xF088, 0xF07B, 0xF06F, 0xF064, 0xF059,
                0xF04F, 0xF046, 0xF03D, 0xF035, 0xF02D, 0xF026, 0xF01F, 0xF019,
                0xF014, 0xF010, 0xF00C, 0xF008, 0xF005, 0xF003, 0xF002, 0xF001,
                0xF001, 0xF001, 0xF002, 0xF003, 0xF005, 0xF008, 0xF00C, 0xF010,
                0xF014, 0xF019, 0xF01F, 0xF026, 0xF02D, 0xF035, 0xF03D, 0xF046,
                0xF04F, 0xF059, 0xF064, 0xF06F, 0xF07B, 0xF088, 0xF095, 0xF0A3,
                0xF0B1, 0xF0C0, 0xF0CF, 0xF0DF, 0xF0F0, 0xF101, 0xF113, 0xF125,
                0xF138, 0xF14C, 0xF160, 0xF175, 0xF18A, 0xF1A0, 0xF1B6, 0xF1CD,
                0xF1E4, 0xF1FC, 0xF215, 0xF22E, 0xF247, 0xF261, 0xF27C, 0xF297,
                0xF2B3, 0xF2CF, 0xF2EC, 0xF309, 0xF327, 0xF345, 0xF363, 0xF383,
                0xF3A2, 0xF3C2, 0xF3E3, 0xF404, 0xF426, 0xF448, 0xF46A, 0xF48D,
                0xF4B0, 0xF4D4, 0xF4F8, 0xF51D, 0xF542, 0xF567, 0xF58D, 0xF5B3,
                0xF5DA, 0xF601, 0xF629, 0xF650, 0xF679, 0xF6A1, 0xF6CA, 0xF6F3,
                0xF71D, 0xF747, 0xF771, 0xF79C, 0xF7C7, 0xF7F2, 0xF81E, 0xF849,
                0xF876, 0xF8A2, 0xF8CF, 0xF8FC, 0xF929, 0xF957, 0xF985, 0xF9B3,
                0xF9E1, 0xFA10, 0xFA3E, 0xFA6D, 0xFA9D, 0xFACC, 0xFAFC, 0xFB2B,
                0xFB5B, 0xFB8C, 0xFBBC, 0xFBED, 0xFC1D, 0xFC4E, 0xFC7F, 0xFCB0,
                0xFCE1, 0xFD13, 0xFD44, 0xFD76, 0xFDA7, 0xFDD9, 0xFE0B, 0xFE3D,
                0xFE6F, 0xFEA1, 0xFED3, 0xFF05, 0xFF38, 0xFF6A, 0xFF9C, 0xFFCE,
        };


INLINE s32 lu_sin(uint theta);
INLINE s32 lu_cos(uint theta);
INLINE uint lu_div(uint x);

INLINE int lu_lerp32(const s32 lut[], uint x, const uint shift);
INLINE int lu_lerp16(const s16 lut[], uint x, const uint shift);

/*! \}	*/

// === POINT ==========================================================

struct RECT;

//!	\addtogroup grpMathPoint
//!	\{

//! 2D Point struct
typedef struct POINT    { int x, y; } POINT, POINT32;


// --- Point functions ---
INLINE POINT *pt_set(POINT *pd, int x, int y);
INLINE POINT *pt_add(POINT *pd, const POINT *pa, const POINT *pb);
INLINE POINT *pt_sub(POINT *pd, const POINT *pa, const POINT *pb);
INLINE POINT *pt_scale(POINT *pd, const POINT *pa, int c);

INLINE POINT *pt_add_eq(POINT *pd, const POINT *pb);
INLINE POINT *pt_sub_eq(POINT *pd, const POINT *pb);
INLINE POINT *pt_scale_eq(POINT *pd, int c);

INLINE int	  pt_cross(const POINT *pa, const POINT *pb);
INLINE int    pt_dot(const POINT *pa, const POINT *pb);

int pt_in_rect(const POINT *pt, const struct RECT *rc);

//!	\}


// === RECT ===========================================================

/*!	\addtogroup grpMathRect		*/
/*!	\{	*/

//! Rectangle struct
typedef struct RECT
{
    int left, top;
    int right, bottom;
} RECT, RECT32;

INLINE RECT *rc_set(RECT *rc, int l, int t, int r, int b);
INLINE RECT *rc_set2(RECT *rc, int x, int y, int w, int h);
INLINE int rc_width(const RECT *rc);
INLINE int rc_height(const RECT *rc);
INLINE RECT *rc_set_pos(RECT *rc, int x, int y);
INLINE RECT *rc_set_size(RECT *rc, int w, int h);
INLINE RECT *rc_move(RECT *rc, int dx, int dy);
INLINE RECT *rc_inflate(RECT *rc, int dw, int dh);
INLINE RECT *rc_inflate2(RECT *rc, const RECT *dr);

RECT *rc_normalize(RECT *rc);

/*!	\}	*/


// === VECTOR =========================================================

/*!	\addtogroup grpMathVector	*/
/*!	\{	*/

//! Vector struct
typedef struct VECTOR   { FIXED x, y, z; } VECTOR;


INLINE VECTOR *vec_set(VECTOR *vd, FIXED x, FIXED y, FIXED z);
INLINE VECTOR *vec_add(VECTOR *vd, const VECTOR *va, const VECTOR *vb);
INLINE VECTOR *vec_sub(VECTOR *vd, const VECTOR *va, const VECTOR *vb);
INLINE VECTOR *vec_mul(VECTOR *vd, const VECTOR *va, const VECTOR *vb);
INLINE VECTOR *vec_scale(VECTOR *vd, const VECTOR *va, FIXED c);
INLINE FIXED vec_dot(const VECTOR *va, const VECTOR *vb);

INLINE VECTOR *vec_add_eq(VECTOR *vd, const VECTOR *vb);
INLINE VECTOR *vec_sub_eq(VECTOR *vd, const VECTOR *vb);
INLINE VECTOR *vec_mul_eq(VECTOR *vd, const VECTOR *vb);
INLINE VECTOR *vec_scale_eq(VECTOR *vd, FIXED c);

VECTOR *vec_cross(VECTOR *vd, const VECTOR *va, const VECTOR *vb);

/*!	\}	*/



// === INLINE =========================================================

// --- General --------------------------------------------------------

//! Get the sign of \a x.
INLINE int sgn(int x)
{	return (x>=0) ? +1 : -1;				}

//! Tri-state sign of \a x: -1 for negative, 0 for 0, +1 for positive.
INLINE int sgn3(int x)
{	return (x>>31) - (-x>>31);				}

//! Get the maximum of \a a and \a b
INLINE int max(int a, int b)
{	return (a > b) ? (a) : (b);				}

//! Get the minimum of \a a and \a b
INLINE int min(int a, int b)
{	return (a < b) ? (a) : (b);				}


//! Range check
INLINE BOOL in_range(int x, int min, int max)
{	return (u32)(x-min) < (u32)(max-min);	}


//! Truncates \a x to stay in range [\a min, \a max>
/*!	\return Truncated value of \a x.
*	\note	\a max is exclusive!
*/
INLINE int clamp(int x, int min, int max)
{	return (x>=max) ? (max-1) : ( (x<min) ? min : x );	}

//! Reflects \a x at boundaries \a min and \a max
/*! If \a x is outside the range [\a min, \a max>,
*	  it'll be placed inside again with the same distance
*	  to the 'wall', but on the other side. Example for lower
*	  border: y = \a min - (\a x- \a min)  = 2*\a min + \a x.
*	\return	Reflected value of \a x.
*	\note	\a max is exclusive!
*/
INLINE int reflect(int x, int min, int max)
{	return (x>=max) ? (2*(max-1)-x)	: ( (x<min) ? (2*min-x)	: x );		}

//! Wraps \a x to stay in range [\a min, \a max>
INLINE int wrap(int x, int min, int max)
{	return (x>=max) ? (x+min-max)	: ( (x<min) ? (x+max-min) : x );	}


// --- Fixed point ----------------------------------------------------


//! Convert an integer to fixed-point
INLINE FIXED int2fx(int d)
{	return d<<FIX_SHIFT;	}

//! Convert a float to fixed-point
INLINE FIXED float2fx(float f)
{	return (FIXED)(f*FIX_SCALEF);	}


//! Convert a FIXED point value to an unsigned integer (orly?).
INLINE u32 fx2uint(FIXED fx)
{	return fx>>FIX_SHIFT;	}

//! Get the unsigned fractional part of a fixed point value (orly?).
INLINE u32 fx2ufrac(FIXED fx)
{	return fx&FIX_MASK;	}

//! Convert a FIXED point value to an signed integer.
INLINE int fx2int(FIXED fx)
{	return fx/FIX_SCALE;	}

//! Convert a fixed point value to floating point.
INLINE float fx2float(FIXED fx)
{	return fx/FIX_SCALEF;	}

//! Add two fixed point values
INLINE FIXED fxadd(FIXED fa, FIXED fb)
{	return fa + fb;			}

//! Subtract two fixed point values
INLINE FIXED fxsub(FIXED fa, FIXED fb)
{	return fa - fb;			}


//! Multiply two fixed point values
INLINE FIXED fxmul(FIXED fa, FIXED fb)
{	return (fa*fb)>>FIX_SHIFT;				}

//! Divide two fixed point values.
INLINE FIXED fxdiv(FIXED fa, FIXED fb)
{	return ((fa)*FIX_SCALE)/(fb);			}


//! Multiply two fixed point values using 64bit math.
INLINE FIXED fxmul64(FIXED fa, FIXED fb)
{	return (((s64)fa)*fb)>>FIX_SHIFT;		}


//! Divide two fixed point values using 64bit math.
INLINE FIXED fxdiv64(FIXED fa, FIXED fb)
{	return ( ((s64)fa)<<FIX_SHIFT)/(fb);	}


// --- LUT ------------------------------------------------------------

//! Look-up a sine value (2&#960; = 0x10000)
/*! \param theta Angle in [0,FFFFh] range
*	 \return .12f sine value
*/
INLINE s32 lu_sin(uint theta)
{	return sin_lut[(theta>>7)&0x1FF];	}

//! Look-up a cosine value (2&#960; = 0x10000)
/*! \param theta Angle in [0,FFFFh] range
*	 \return .12f cosine value
*/
INLINE s32 lu_cos(uint theta)
{	return sin_lut[((theta>>7)+128)&0x1FF];	}

//! Look-up a division value between 0 and 255
/*! \param x reciprocal to look up.
*	 \return 1/x (.16f)
*/
INLINE uint lu_div(uint x)
{	return 0;	}


//! Linear interpolator for 32bit LUTs.
/*! A lut is essentially the discrete form of a function, f(<i>x</i>).
*	You can get values for non-integer \e x via (linear)
*	interpolation between f(x) and f(x+1).
*	\param lut	The LUT to interpolate from.
*	\param x	Fixed point number to interpolate at.
*	\param shift	Number of fixed-point bits of \a x.
*/
INLINE int lu_lerp32(const s32 lut[], uint x, const uint shift)
{
    int xa, ya, yb;
    xa=x>>shift;
    ya= lut[xa]; yb= lut[xa+1];
    return ya + ( (yb-ya)*(x-(xa<<shift))>>shift );
}

//! As lu_lerp32, but for 16bit LUTs.
INLINE int lu_lerp16(const s16 lut[], uint x, const uint shift)
{
    int xa, ya, yb;
    xa=x>>shift;
    ya= lut[xa]; yb= lut[xa+1];
    return ya + ( (yb-ya)*(x-(xa<<shift))>>shift );
}


// --- Point ----------------------------------------------------------

//! Initialize \a pd to (\a x, \a y)
INLINE POINT *pt_set(POINT *pd, int x, int y)
{
    pd->x= x;	pd->y= y;
    return pd;
}

//! Point addition: \a pd = \a pa + \a pb
INLINE POINT *pt_add(POINT *pd, const POINT *pa, const POINT *pb)
{
    pd->x= pa->x + pb->x;
    pd->y= pa->x + pb->y;
    return pd;
}

//! Point subtraction: \a pd = \a pa - \a pb
INLINE POINT *pt_sub(POINT *pd, const POINT *pa, const POINT *pb)
{
    pd->x= pa->x - pb->x;
    pd->y= pa->x - pb->y;
    return pd;
}

//! Point scale: \a pd = \a c * \a pa
INLINE POINT *pt_scale(POINT *pd, const POINT *pa, int c)
{
    pd->x= pa->x*c;
    pd->y= pa->y*c;
    return pd;
}

//! Point  increment: \a pd += \a pb
INLINE POINT *pt_add_eq(POINT *pd, const POINT *pb)
{
    pd->x += pb->y;
    pd->y += pb->y;
    return pd;
}

//! Point decrement: \a pd -= \a pb
INLINE POINT *pt_sub_eq(POINT *pd, const POINT *pb)
{
    pd->x -= pb->y;
    pd->y -= pb->y;
    return pd;
}

//! Point scale: \a pd *= \a c
INLINE POINT *pt_scale_eq(POINT *pd, int c)
{
    pd->x *= c;
    pd->y *= c;
    return pd;
}

//! Point 'cross'-product: \a pa \htmlonly &times; \endhtmlonly \a pb
/*! Actually, there's no such thing as a 2D cross-product, but you could
*	  extend it to 3D and get the value of its <i>z</i>-component,
*	  which can be used for a test for parallelism.
*/
INLINE int pt_cross(const POINT *pa, const POINT *pb)
{	return pa->x * pb->y - pa->y * pb->x;			}


//! Point 'dot'-product:\a pa \htmlonly &middot; \endhtmlonly \a pb
INLINE int pt_dot(const POINT *pa, const POINT *pb)
{	return pa->x * pb->x + pa->y * pb->y;			}



// --- Rect -----------------------------------------------------------

//! Initialize a rectangle.
/*!	\param l	Left side.
*	\param t	Top side.
*	\param r	Right side.
*	\param b	Bottom side.
*/
INLINE RECT *rc_set(RECT *rc, int l, int t, int r, int b)
{
    rc->left= l; rc->top= t; rc->right= r; rc->bottom= b;
    return rc;
}

//! Initialize a rectangle, with sizes inside of max boundaries.
/*!	\param x	Left side.
*	\param y	Top side.
*	\param w	Width.
*	\param h	Height.
*/
INLINE RECT *rc_set2(RECT *rc, int x, int y, int w, int h)
{
    rc->left= x; rc->top= y; rc->right= x+w; rc->bottom= y+h;
    return rc;
}

//! Get rectangle width.
INLINE int rc_width(const RECT *rc)
{	return rc->right - rc->left;	}

//! Get rectangle height
INLINE int rc_height(const RECT *rc)
{	return rc->bottom - rc->top;	}

//! Move rectangle to (\a x, \a y) position.
INLINE RECT *rc_set_pos(RECT *rc, int x, int y)
{
    rc->right += x-rc->left;	rc->left= x;
    rc->bottom += y-rc->top;	rc->top= y;
    return rc;
}

//! Reside rectangle.
INLINE RECT *rc_set_size(RECT *rc, int w, int h)
{
    rc->right= rc->left+w;		rc->bottom= rc->top+h;
    return rc;
}

//! Move rectangle by (\a dx, \a dy).
INLINE RECT *rc_move(RECT *rc, int dx, int dy)
{
    rc->left += dx;		rc->top += dy;
    rc->right += dx;	rc->bottom += dy;
    return rc;
}

//! Increase size by \a dw horizontally and \a dh vertically.
INLINE RECT *rc_inflate(RECT *rc, int dw, int dh)
{
    rc->left -= dw;		rc->top -= dh;
    rc->right += dw;	rc->bottom += dh;
    return rc;
}

//! Increase sizes on all sides by values of rectangle \a dr.
INLINE RECT *rc_inflate2(RECT *rc, const RECT *dr)
{
    rc->left += dr->left;	rc->top += dr->top;
    rc->right += dr->right;	rc->bottom += dr->bottom;
    return rc;
}


// --- Vector ---------------------------------------------------------

//! Initialize a vector
INLINE VECTOR *vec_set(VECTOR *vd, FIXED x, FIXED y, FIXED z)
{
    vd->x= x; vd->y= y; vd->z= z;
    return vd;
}

//! Add vectors: \b d = \b a + \b b;
INLINE VECTOR *vec_add(VECTOR *vd, const VECTOR *va, const VECTOR *vb)
{
    vd->x= va->x + vb->x;
    vd->y= va->y + vb->y;
    vd->z= va->z + vb->z;
    return vd;
}

//! Subtract vectors: \b d = \b a - \b b;
INLINE VECTOR *vec_sub(VECTOR *vd, const VECTOR *va, const VECTOR *vb)
{
    vd->x= va->x - vb->x;
    vd->y= va->y - vb->y;
    vd->z= va->z - vb->z;
    return vd;
}

//! Multiply vectors elements: \b d = \b S(ax, ay, az) �\b b
INLINE VECTOR *vec_mul(VECTOR *vd, const VECTOR *va, const VECTOR *vb)
{
    vd->x= fxmul(va->x, vb->x);
    vd->y= fxmul(va->y, vb->y);
    vd->z= fxmul(va->z, vb->z);
    return vd;
}

//! Scale vector: \b d = c*\b a
INLINE VECTOR *vec_scale(VECTOR *vd, const VECTOR *va, FIXED c)
{
    vd->x= fxmul(va->x, c);
    vd->y= fxmul(va->y, c);
    vd->z= fxmul(va->z, c);
    return vd;
}

//! Dot-product: d = \b a �\b b
INLINE FIXED vec_dot(const VECTOR *va, const VECTOR *vb)
{
    FIXED dot;
    dot  = fxmul(va->x, vb->x);
    dot += fxmul(va->y, vb->y);
    dot += fxmul(va->z, vb->z);
    return dot;
}

//! Increment vector: \b d += \b b;
INLINE VECTOR *vec_add_eq(VECTOR *vd, const VECTOR *vb)
{	vd->x += vb->x;	vd->y += vb->y;	vd->z += vb->z;	return vd;	}

//! Decrease vector: \b d -= \b b;
INLINE VECTOR *vec_sub_eq(VECTOR *vd, const VECTOR *vb)
{	vd->x -= vb->x;	vd->y -= vb->y;	vd->z -= vb->z;	return vd;	}

//! Multiply vectors elements: \b d = \b S(dx, dy, dz) �\b b
INLINE VECTOR *vec_mul_eq(VECTOR *vd, const VECTOR *vb)
{
    vd->x= fxmul(vd->x, vb->x);
    vd->y= fxmul(vd->y, vb->y);
    vd->z= fxmul(vd->z, vb->z);
    return vd;
}

//! Scale vector: \b d = c*\b d
INLINE VECTOR *vec_scale_eq(VECTOR *vd, FIXED c)
{
    vd->x= fxmul(vd->x, c);
    vd->y= fxmul(vd->y, c);
    vd->z= fxmul(vd->z, c);
    return vd;
}


#endif //GBA_SPRITE_ENGINE_PROJECT_TONC_MATH_STUB_H
