//
// Created by Wouter Groeneveld on 28/07/18.
//

#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <cstring>
#include <libgba-sprite-engine/gba/tonc_core.h>
#include <libgba-sprite-engine/matrixfx.h>
#include <libgba-sprite-engine/background/text_stream.h>

std::unique_ptr<SoundControl> GBAEngine::activeChannelA;
std::unique_ptr<SoundControl> GBAEngine::activeChannelB;
std::unique_ptr<Timer> GBAEngine::timer;

u16 *vid_page;

void GBAEngine::vsync() {
    while (REG_VCOUNT >= 160);
    while (REG_VCOUNT < 160);
}

Timer* GBAEngine::getTimer() {
    return GBAEngine::timer.get();
}

void GBAEngine::onVBlank() {
    // WARNING this is a very dangerous piece of code.
    // GBA IRQs seem eager to crash or eat up CPU. Get in, disable stuff, work, enable, get out!
    stopOnVBlank();

    unsigned short tempInterruptState = REG_IF;

    if((REG_IF & INTERRUPT_VBLANK) == INTERRUPT_VBLANK) {
        GBAEngine::timer->onvblank();

        if(GBAEngine::activeChannelA) {
            if(GBAEngine::activeChannelA->done()) {
                GBAEngine::activeChannelA->reset();
            } else {
                GBAEngine::activeChannelA->step();
            }
        }
        if(GBAEngine::activeChannelB) {
            if(GBAEngine::activeChannelB->done()) {
                GBAEngine::activeChannelB->disable();
                GBAEngine::activeChannelB = nullptr;    // never delete, let unique_ptr do that, known to flip here
            } else {
                GBAEngine::activeChannelB->step();
            }
        }
    }

    REG_IF = tempInterruptState;
    startOnVBlank();
}

u16 GBAEngine::readKeys() {
    return ~REG_KEYS & KEY_ANY;
}

void GBAEngine::dequeueAllSounds() {
    if(GBAEngine::activeChannelA) {
        GBAEngine::activeChannelA->disable();
    } if(GBAEngine::activeChannelB) {
        GBAEngine::activeChannelB->disable();
    }
}

void GBAEngine::enqueueSound(const s8 *data, int totalSamples, int sampleRate, SoundChannel channel) {
    SoundControl* control;

    if(channel == ChannelA) {                       // repeating bg music can be restarted
        GBAEngine::activeChannelA = SoundControl::channelAControl();
        control = GBAEngine::activeChannelA.get();
    } else {                                        // sound still playing, don't stop that
        if(GBAEngine::activeChannelB) {
            if(!GBAEngine::activeChannelB->done()) return;
            GBAEngine::activeChannelB = nullptr;
        }
        GBAEngine::activeChannelB = SoundControl::channelBControl();
        control = GBAEngine::activeChannelB.get();
    }

    disableTimer0AndVBlank();
    control->disable();

    REG_SNDDSCNT |= control->getControlFlags();     // output to both sides, reset fifo
    REG_SNDSTAT = SSTAT_ENABLE;                     // enable all sound
    u16 ticksPerSample = CLOCK / sampleRate;        // divide the clock (ticks/second) by the sample rate (samples/second)

    control->accept(data, totalSamples, ticksPerSample);
    control->enable();

    REG_TM0D = OVERFLOW_16_BIT_VALUE - ticksPerSample;

    enableTimer0AndVBlank();
}

void GBAEngine::disableTimer0AndVBlank() {
    stopOnVBlank();
    REG_TM0CNT = 0;
}

void GBAEngine::enableTimer0AndVBlank() {
    REG_TM0CNT = TM_ENABLE | TM_FREQ_1;             // enable timer - dma auto-syncs to this thanks to DMA_SYNC_TO_TIMER
    startOnVBlank();
}

GBAEngine::GBAEngine() {
    GBAEngine::timer = std::unique_ptr<Timer>(new Timer());
    // setup screen control flags
    REG_DISPCNT = DCNT_MODE4 | DCNT_OBJ | DCNT_OBJ_1D | DCNT_BG2;

    // setup interrupt control flags for vblank IRQing (started only when sound played)
    REG_DISPSTAT |= DISPLAY_INTERRUPT_VBLANK_ENABLE;
    REG_IE |= INTERRUPT_VBLANK;
    *IRQ_CALLBACK = (u32) &GBAEngine::onVBlank;

    enableTimer0AndVBlank();

    REG_SNDDSCNT = 0;
    vid_page = vid_mem_back;
    projectionMatrix = MatrixFx::perspectiveFovLH(float2fx(0.78), fxdiv(GBA_SCREEN_WIDTH_FX, GBA_SCREEN_HEIGHT_FX), float2fx(0.01), ONE);
}

void GBAEngine::update() {
    // main update loop, in while(true) {}.
    // WARNING - keep amount of instructions as minimal as possible in here!

    u16 keys = readKeys();
    // main scene update loop call. This *might* take a while.
    currentScene->tick(keys);

    // TODO use software interrupt Vsyncing instead of 2 wasteful whiles
    vsync();
    renderClear();
    render();
    flipPage();
}

void GBAEngine::flipPage() {
    // toggle the write_buffer's page
    vid_page= (COLOR*)((u32)vid_page ^ VRAM_PAGE_SIZE);
    REG_DISPCNT ^= DCNT_PAGE;
}

// http://www.coranac.com/tonc/text/bitmaps.htm
// this thing is supposed to be very slow. see link above.
inline void GBAEngine::plotPixel(int x, int y, u8 clrId) {
    u16 *dst = &vid_page[(y * M4_WIDTH + x) / 2];
    if(x & 1) {
        *dst = (*dst & 0xFF) | (clrId << 8);
    } else {
        *dst = (*dst & ~0xFF) | clrId;
    }
}

inline VectorFx GBAEngine::project(const VectorFx &coord, const MatrixFx &transMat) {
    auto point = MatrixFx::transformCoordinates(coord, transMat);

    auto x = fxmul(point.x(), GBA_SCREEN_WIDTH_FX) + fxdiv(GBA_SCREEN_WIDTH_FX, TWO);
    auto y = fxmul(-point.y(), GBA_SCREEN_HEIGHT_FX) + fxdiv(GBA_SCREEN_HEIGHT_FX, TWO);
    return VectorFx(x, y, 0);
}

// does the mesh rendering - to write mem before flipping
void GBAEngine::render() {
    auto viewMatrix = MatrixFx::lookAtLH(currentCamera.getPosition(), currentCamera.getTarget(), VectorFx::up());

    for(auto& mesh :currentScene->meshes()) {
        auto worldMatrix = MatrixFx::rotationYawPitchRoll(mesh->roty(), mesh->rotx(), mesh->rotz()) * MatrixFx::translation(mesh->position());
        auto transformMatrix = worldMatrix * viewMatrix * projectionMatrix;

        TextStream::instance().setText(transformMatrix.to_string_m1(), 1, 1);
        TextStream::instance().setText(transformMatrix.to_string_m2(), 2, 1);
        TextStream::instance().setText(transformMatrix.to_string_m3(), 3, 1);
        TextStream::instance().setText(transformMatrix.to_string_m4(), 4, 1);

        int i = 0;
        for(auto& vertex : mesh->vertices()) {
            auto projectedPoint = project(*vertex.get(), transformMatrix).toInt();
            plotPixel(projectedPoint.x(), projectedPoint.y(), 1);

            //TextStream::instance().setText(std::to_string(i) + ":" + projectedPoint.to_string(), i, 1);
            i++;
        }
    }

    /*
     * according to the unit tests; it should do something like this:
    plotPixel(150, 40, 1);
    plotPixel(60, 40, 1);
    plotPixel(150, 140, 1);
    plotPixel(93, 26, 1);
    plotPixel(93, 115, 1);
    plotPixel(172, 115, 1);
    plotPixel(60, 140, 1);
    plotPixel(93, 26, 1);
    */
}

void GBAEngine::renderClear() {
    dma3_cpy(vid_page, black, VRAM_PAGE_SIZE);
}

void GBAEngine::cleanupPreviousScene()  {
    delete currentScene;
    sceneToTransitionTo = nullptr;
}

void GBAEngine::setScene(Scene* scene) {
    dequeueAllSounds();

    if(this->currentScene) {
        cleanupPreviousScene();
    }
    scene->load();

    auto fgPalette = scene->getForegroundPalette();
    fgPalette->persist();
    auto bgPalette = scene->getBackgroundPalette();
    bgPalette->persist();

    this->currentScene = scene;
    this->currentCamera = this->currentScene->camera();
}
