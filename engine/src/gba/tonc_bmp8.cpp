//
// Created by Wouter Groeneveld on 10/07/20.
//

//! Plot a single pixel on a 8-bit buffer
#include <libgba-sprite-engine/gba/tonc_types.h>
#include <libgba-sprite-engine/gba/tonc_core.h>
#include <libgba-sprite-engine/gba/toolbox.h>

/*!
	\param x		X-coord.
	\param y		Y-coord.
	\param clr		Color.
	\param dstBase	Canvas pointer (halfword-aligned plz).
	\param dstP		Canvas pitch in bytes.
	\note	Slow as fuck. Inline plotting functionality if possible.
*/
void bmp8_plot(int x, int y, u32 clr, void *dstBase, uint dstP)
{
    u16 *dstD= (u16*)(dstBase+y*dstP+(x&~1));

    if(x&1)
        *dstD= (*dstD& 0xFF) | (clr<<8);
    else
        *dstD= (*dstD&~0xFF) | (clr&0xFF);
}


//! Draw a horizontal line on an 8bit buffer
/*!
	\param x1		First X-coord.
	\param y		Y-coord.
	\param x2		Second X-coord.
	\param clr		Color index.
	\param dstBase	Canvas pointer (halfword-aligned plz).
	\param dstP		canvas pitch in bytes.
	\note	Does normalization, but not bounds checks.
*/
void bmp8_hline(int x1, int y, int x2, u32 clr, void *dstBase, uint dstP)
{
    // --- Normalize ---
    clr &= 0xFF;
    if(x2<x1)
    {	int tmp= x1; x1= x2; x2= tmp;	}

    uint width= x2-x1+1;
    u16 *dstL= (u16*)(dstBase+y*dstP + (x1&~1));

    // --- Left unaligned pixel ---
    if(x1&1)
    {
        *dstL= (*dstL & 0xFF) + (clr<<8);
        width--;
        dstL++;
    }

    // --- Right unaligned pixel ---
    if(width&1)
        dstL[width/2]= (dstL[width/2]&~0xFF) + clr;
    width /= 2;

    // --- Aligned line ---
    if(width)
        memset16(dstL, dup8(clr), width);
}


//! Draw a vertical line on an 8bit buffer
/*!
	\param x		X-coord.
	\param y1		First Y-coord.
	\param y2		Second Y-coord.
	\param clr		Color index.
	\param dstBase	Canvas pointer (halfword-aligned plz).
	\param dstP		canvas pitch in bytes.
	\note	Does normalization, but not bounds checks.
*/
void bmp8_vline(int x, int y1, int y2, u32 clr, void *dstBase, uint dstP)
{
    // --- Normalize ---
    if(y2<y1)
    {	int tmp= y1; y1= y2; y2= tmp;	}

    uint height= y2-y1+1;
    u16 *dstL= (u16*)(dstBase+y1*dstP + (x&~1));
    dstP /= 2;

    if(x&1)
    {
        clr <<= 8;
        while(height--)
        {	*dstL= (*dstL& 0xFF) + clr;	dstL += dstP;	}
    }
    else
    {
        clr &= 0xFF;
        while(height--)
        {	*dstL= (*dstL&~0xFF) + clr;	dstL += dstP;	}
    }
}


//! Draw a line on an 8bit buffer
/*!
	\param x1		First X-coord.
	\param y1		First Y-coord.
	\param x2		Second X-coord.
	\param y2		Second Y-coord.
	\param clr		Color index.
	\param dstBase	Canvas pointer (halfword-aligned plz).
	\param dstP		Canvas pitch in bytes.
	\note	Does normalization, but not bounds checks.
*/
void bmp8_line(int x1, int y1, int x2, int y2, u32 clr,
               void *dstBase, uint dstP)
{

    // Trivial lines: horz and vertical
    if(y1 == y2)		// Horizontal
    {
        // Adjust for inclusive ends
        if(x2 == x1)
        {	bmp8_plot(x1, y1, clr, dstBase, dstP);	return;	}

        bmp8_hline(x1, y1, x2, clr, dstBase, dstP);
        return;
    }
    else if(x1 == x2)	// Vertical
    {
        // Adjust for inclusive ends
        if(y2 == y1)
        {	bmp8_plot(x1, y1, clr, dstBase, dstP);	return;	}

        bmp8_vline(x1, y1, y2, clr, dstBase, dstP);
        return;
    }

    // --- Slogging through the diagonal ---

    int ii, dx, dy, xstep, ystep, dd;
    u32 addr= (u32)(dstBase + y1*dstP + x1), mask= 255;
    u16 *dstL;

    clr &= mask;
    clr |= clr<<8;
    if(x1 & 1)
        mask= ~mask;

    // --- Normalization ---
    if(x1>x2)
    {	xstep= -1;	dx= x1-x2;	}
    else
    {	xstep= +1;	dx= x2-x1;	}

    if(y1>y2)
    {	ystep= -dstP;	dy= y1-y2;	}
    else
    {	ystep= +dstP;	dy= y2-y1;	}


    // --- Drawing ---
    // NOTE: because xstep is alternating, you can do marvels
    //	with mask-flips
    // NOTE: (mask>>31) is equivalent to (x&1) ? 0 : 1

    if(dx>=dy)		// Diagonal, slopeFx <= 1
    {
        dd= 2*dy - dx;

        for(ii=dx; ii>=0; ii--)
        {
            dstL= (u16*)(addr - (mask>>31));
            *dstL= (*dstL &~ mask) | (clr & mask);

            if(dd >= 0)
            {	dd -= 2*dx;	addr += ystep;	}

            dd += 2*dy;
            addr += xstep;
            mask = ~mask;
        }
    }
    else				// # Diagonal, slopeFx > 1
    {
        dd= 2*dx - dy;

        for(ii=dy; ii>=0; ii--)
        {
            dstL= (u16*)(addr - (mask>>31));
            *dstL= (*dstL &~ mask) | (clr & mask);

            if(dd >= 0)
            {
                dd -= 2*dy;
                addr += xstep;
                mask = ~mask;
            }

            dd += 2*dx;
            addr += ystep;
        }
    }
}

INLINE void m4_line(int x1, int y1, int x2, int y2, u8 clrid)
{	bmp8_line(x1, y1, x2, y2, clrid, vid_page, M4_WIDTH);				}
