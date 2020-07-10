//
// Created by Wouter Groeneveld on 10/07/20.
// Excerpts from tonc_video - only taken which was needed (M4)
//

#ifndef GBA_BITMAP_ENGINE_PROJECT_TONC_VIDEO_H
#define GBA_BITMAP_ENGINE_PROJECT_TONC_VIDEO_H

#include <libgba-sprite-engine/gba/tonc_types.h>
#include <libgba-sprite-engine/gba/toolbox.h>

void bmp8_plot(int x, int y, u32 clr, void *dstBase, uint dstP);

void bmp8_hline(int x1, int y, int x2, u32 clr, void *dstBase, uint dstP);
void bmp8_vline(int x, int y1, int y2, u32 clr, void *dstBase, uint dstP);
void bmp8_line(int x1, int y1, int x2, int y2, u32 clr,
               void *dstBase, uint dstP);

INLINE void m4_hline(int x1, int y, int x2, u8 clrid);
INLINE void m4_vline(int x, int y1, int y2, u8 clrid);
INLINE void m4_line(int x1, int y1, int x2, int y2, u8 clrid);
INLINE void m4_plot(int x, int y, u8 clrid);

//! Plot a \a clrid pixel on the current mode 4 backbuffer
INLINE void m4_plot(int x, int y, u8 clrid)
{
    u16 *dst= &vid_page[(y*M4_WIDTH+x)>>1];
    if(x&1)
        *dst= (*dst& 0xFF) | (clrid<<8);
    else
        *dst= (*dst&~0xFF) |  clrid;
}


//! Draw a \a clrid colored horizontal line in mode 4.
INLINE void m4_hline(int x1, int y, int x2, u8 clrid)
{	bmp8_hline(x1, y, x2, clrid, vid_page, M4_WIDTH);					}


//! Draw a \a clrid colored vertical line in mode 4.
INLINE void m4_vline(int x, int y1, int y2, u8 clrid)
{	bmp8_vline(x, y1, y2, clrid, vid_page, M4_WIDTH);					}


//! Draw a \a clrid colored line in mode 4.
INLINE void m4_line(int x1, int y1, int x2, int y2, u8 clrid)
{	bmp8_line(x1, y1, x2, y2, clrid, vid_page, M4_WIDTH);				}

#endif //GBA_BITMAP_ENGINE_PROJECT_TONC_VIDEO_H
