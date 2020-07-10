//
// Created by Wouter Groeneveld on 28/07/18.
//

#ifndef GBA_SPRITE_ENGINE_GBAENGINE_H
#define GBA_SPRITE_ENGINE_GBAENGINE_H


#include <libgba-sprite-engine/gba/tonc_memmap.h>
#include <libgba-sprite-engine/gba/tonc_memmap.h>
#include <libgba-sprite-engine/vectorfx.h>
#include <libgba-sprite-engine/vectorpx.h>
#include <libgba-sprite-engine/matrixfx.h>
#include "scene.h"
#include "sound_control.h"
#include "timer.h"


const unsigned int black[VRAM_PAGE_SIZE] = {};

class GBAEngine {
private:
    // WHY raw pointers? the engine does the transition and cleanup work itself
    Scene* currentScene;
    Camera currentCamera;
    Scene* sceneToTransitionTo;

    MatrixFx projectionMatrix;

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
    inline VectorPx project(const VectorFx &coord, const MatrixFx &transMat);
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
    inline void delay(int times) {
        for(int i = 0; i < times; i++){}
    }

    inline void plotPixel(int x, int y, u8 clrId);
    inline void plotPixel(const VectorPx &pixel, u8 clrId);
    inline void plotLine(const VectorPx &point0, const VectorPx &point1, u8 clrId);
};


#endif //GBA_SPRITE_ENGINE_GBAENGINE_H
