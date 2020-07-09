//
// Created by Wouter Groeneveld on 28/07/18.
//

#ifndef GBA_BITMAP_ENGINE_TEXT_STREAM_H
#define GBA_BITMAP_ENGINE_TEXT_STREAM_H

#include <string>
#include <memory>

#define TEXT_WIDTH 8

// tonc_text.h assemly ref
extern const u32 toncfontTiles[192];
#define toncfontTilesLen 768
extern u16 *vid_page;

typedef struct tagTXT_BASE
{
    u16 *dst0;      //!< writing buffer starting point
    u32 *font;      // pointer to font used
    u8 *chars;     // character map (chars as in letters, not tiles)
    u8 *cws;       // char widths (for VWF)
    u8  dx,dy;     // letter distances
    u16  flags;     // for later
    u8  extra[12]; // ditto
} ALIGN4 TXT_BASE;


class TextStream {
private:
    int currRow, currCol;
    short currColorIndex;

    u8 txt_lut[256];

    TXT_BASE __txt_base;
    TXT_BASE *gptxt;

    static TextStream* inst;
    TextStream();
    TextStream(TextStream& other) = delete;
    TextStream(TextStream&& other) = delete;

public:
    void clear();
    void setText(std::string text, int row, int col);
    void setText(const char* text, int row, int col);

    inline void setFontColorIndex(short index) { currColorIndex = index; }

    static TextStream& instance();

    TextStream& operator << (const char* s);
    TextStream& operator << (const int s);
    TextStream& operator << (const u32 s);
    TextStream& operator << (const bool s);
};


#endif //GBA_SPRITE_ENGINE_TEXT_STREAM_H
