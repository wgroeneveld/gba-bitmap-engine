//
// Created by Wouter Groeneveld on 28/07/18.
//

#ifndef GBA_SPRITE_ENGINE_GBAENGINE_H
#define GBA_SPRITE_ENGINE_GBAENGINE_H


#include <libgba-sprite-engine/gba/tonc_memmap.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include "scene.h"
#include "sound_control.h"
#include "timer.h"

#define GBA_SCREEN_WIDTH 240
#define GBA_SCREEN_HEIGHT 160
#define M4_WIDTH 240

const unsigned int black[VRAM_PAGE_SIZE] = {};

class GBAEngine {
private:
    // WHY raw pointers? the engine does the transition and cleanup work itself
    Scene* currentScene;
    Scene* sceneToTransitionTo;

    u16* vid_page;

    static std::unique_ptr<Timer> timer;
    static std::unique_ptr<SoundControl> activeChannelA;
    static std::unique_ptr<SoundControl> activeChannelB;

    void vsync();
    void cleanupPreviousScene();
    void enqueueSound(const s8 *data, int totalSamples, int sampleRate, SoundChannel channel);

    void enableTimer0AndVBlank();
    void disableTimer0AndVBlank();
    static void startOnVBlank() { REG_IME = 1; }
    static void stopOnVBlank() { REG_IME = 0; }
    static void onVBlank();

    void render();
    void renderClear();
    inline void plotPixel(int x, int y, u8 clrId);
    void flipPage();

public:
    GBAEngine();

    Timer* getTimer();
    void setScene(Scene* scene);

    void dequeueAllSounds();
    void enqueueMusic(const s8 *data, int totalSamples, int sampleRate = 16000) {
        enqueueSound(data, totalSamples, sampleRate, ChannelA);
    }
    void enqueueSound(const s8 *data, int totalSamples, int sampleRate = 16000) {
        enqueueSound(data, totalSamples, sampleRate, ChannelB);
    }

    u16 readKeys();
    void update();
    void delay(int times) {
        for(int i = 0; i < times; i++){}
    }
};


#endif //GBA_SPRITE_ENGINE_GBAENGINE_H
