//
// Created by Wouter Groeneveld on 28/07/18.
//

#include <libgba-sprite-engine/gba/tonc_memmap.h>
#include <libgba-sprite-engine/palette/palette_manager.h>
#include <libgba-sprite-engine/background/text_stream.h>

#include <memory>

TextStream* TextStream::inst;

void TextStream::clear() {
    currRow = 0;
    currCol = 0;
    currColorIndex = 1;
    // can't actually 'clear' stuff in bitmap-mode, unless we fuck up everything else!

    gptxt->dx= gptxt->dy= 8;

    gptxt->dst0= vid_mem;
    gptxt->font= (u32*)toncfontTiles;
    gptxt->chars= txt_lut;
    gptxt->cws= NULL;

    int ii;
    for(ii=0; ii<96; ii++)
        gptxt->chars[ii+32]= ii;
}

TextStream::TextStream() {
    gptxt = &__txt_base;
    clear();
}

TextStream& TextStream::instance() {
    if(!inst) {
        inst = new TextStream();
    }
    return *inst;
}

void TextStream::setText(std::string text, int row, int col) {
    setText(text.c_str(), row, col);
}

// thank you cern/tonclib
void TextStream::setText(const char* text, int yPos, int xPos) {
    /*
    INLINE void m4_puts(int x, int y, const char *str, u8 clrid)
    {	bm8_puts(&vid_page[(y*240+x)>>1], str, clrid);	}
     */

    u16 *dst = &vid_page[(yPos*240*TEXT_WIDTH+xPos)>>1];
    int c, x=0, dx= gptxt->dx>>1;

    while((c=*text++) != 0)
    {
        if(c == '\n')		// line break
        {
            dst += 120*gptxt->dy;
            x=0;
        }
        else				// normal character
        {
            int ix, iy;
            u32 row, pxs;

            // point to glyph; each line is one byte
            u8 *pch= (u8*)&gptxt->font[2*gptxt->chars[c]];
            for(iy=0; iy<8; iy++)
            {
                row= pch[iy];
                for(ix=x; row>0; row >>= 2, ix++)
                {
                    pxs= dst[iy*120+ix];
                    if(row&1)
                        pxs= (pxs&0xFF00) | currColorIndex;
                    if(row&2)
                        pxs= (pxs&0x00FF) | (currColorIndex<<8);

                    dst[iy*120+ix]= pxs;
                }
            }
            x += dx;
        }
    }
}

TextStream& TextStream::operator<<(const int s) {
    return *this << std::to_string(s).c_str();
}

TextStream& TextStream::operator<<(const u32 s) {
    return *this << std::to_string(s).c_str();
}

TextStream& TextStream::operator<<(const bool s) {
    return *this << (s ? "TRUE" : "FALSE");
}

TextStream& TextStream::operator<<(const char * s) {
    setText(s, currRow, currCol);
    currRow++;
    return *this;
}
