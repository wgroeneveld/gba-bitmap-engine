//
// Created by Wouter Groeneveld on 28/07/18.
//

#ifndef GBA_SPRITE_ENGINE_GBAENGINE_H
#define GBA_SPRITE_ENGINE_GBAENGINE_H


#include <libgba-sprite-engine/gba/tonc_memmap.h>
#include <libgba-sprite-engine/gba/tonc_memmap.h>
#include <libgba-sprite-engine/vectorfx.h>
#include <libgba-sprite-engine/matrixfx.h>
#include <libgba-sprite-engine/renderer/renderer.h>
#include "libgba-sprite-engine/scene.h"
#include "libgba-sprite-engine/sound_control.h"
#include "libgba-sprite-engine/timer.h"


const unsigned int black[VRAM_PAGE_SIZE] = {};

class GBAEngine {
private:
    // WHY raw pointers? the engine does the transition and cleanup work itself
    Scene* currentScene;
    Camera currentCamera;
    Scene* sceneToTransitionTo;

    bool showfps;
    void showFPS();
    MatrixFx projectionMatrix;
    std::unique_ptr<Renderer> renderer;

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

    void setRenderer(Renderer* r) { renderer = std::unique_ptr<Renderer>(r); };

    VectorFx project(const VectorFx &coord, const MatrixFx &transMat);
    void plotPixel(int x, int y, u8 clrId);
    void plotPixel(const VectorFx &pixel, u8 clrId);
    void plotLine(const VectorFx &point0, const VectorFx &point1, u8 clrId);
    void plotLine(int p0x, int p0y, int p1x, int p1y, u8 clrId);
};


#endif //GBA_SPRITE_ENGINE_GBAENGINE_H
