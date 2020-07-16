//
// Created by Wouter Groeneveld on 16/07/20.
//

#ifndef GBA_BITMAP_ENGINE_PROJECT_TEXTURE_H
#define GBA_BITMAP_ENGINE_PROJECT_TEXTURE_H


//{{BLOCK(questionmark)

//======================================================================
//
//	questionmark, 16x16@16,
//	+ 4 tiles not compressed
//	Total size: 512 = 512
//
//	Time-stamp: 2020-07-15, 20:38:24
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

// BLUE: 2
// DARK: 3
// LIGHT: 4
// BLACK: 0

#define QUESTIONMARK_TILES_LENGTH 256

const unsigned short questionmarkTiles[QUESTIONMARK_TILES_LENGTH] __attribute__((aligned(4)))=
        {
                2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2,
                3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0,
                3, 4, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 4, 0,
                3, 4, 4, 4, 4, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 0,
                3, 4, 4, 4, 3, 3, 0, 0, 3, 3, 3, 0, 4, 4, 4, 0,
                3, 4, 4, 4, 3, 3, 0, 4, 3, 3, 3, 0, 4, 4, 4, 0,
                3, 4, 4, 4, 3, 3, 0, 4, 3, 3, 3, 0, 4, 4, 4, 0,
                3, 4, 4, 4, 4, 0, 0, 4, 3, 0, 0, 0, 4, 4, 4, 0,
                3, 4, 4, 4, 4, 4, 4, 3, 3, 0, 0, 0, 4, 4, 4, 0,
                3, 4, 4, 4, 4, 4, 4, 3, 3, 0, 4, 4, 4, 4, 4, 0,
                3, 4, 4, 4, 4, 4, 4, 4, 0, 0, 4, 4, 4, 4, 4, 0,
                3, 4, 4, 4, 4, 4, 4, 3, 3, 4, 4, 4, 4, 4, 4, 0,
                3, 4, 4, 4, 4, 4, 4, 3, 3, 0, 4, 4, 4, 4, 4, 0,
                3, 4, 0, 4, 4, 4, 4, 4, 0, 0, 4, 4, 4, 0, 4, 0,
                3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        };

//}}BLOCK(questionmark)

//{{BLOCK(Shared)

//======================================================================
//
//	Shared, 16x16@8,
//	+ palette 4 entries, not compressed
//	Total size: 8 = 8
//
//	Time-stamp: 2020-07-15, 20:38:24
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

unsigned short color(unsigned int r, unsigned int g, unsigned int b) {
        r = (float) r / 255 * 31;
        g = (float) g / 255 * 31;
        b = (float) b / 255 * 31;

        unsigned short c = (b & 0x1f) << 10;
        c |= (g & 0x1f) << 5;
        c |= (r & 0x1f);
        return c;
}

const unsigned short sharedPal[] __attribute__((aligned(4)))=
        {
                0x0000, color(255, 255, 255), color(92, 148, 252), color(200, 76, 12), color(252, 152, 56)
        };

//}}BLOCK(Shared)


#endif //GBA_BITMAP_ENGINE_PROJECT_TEXTURE_H
