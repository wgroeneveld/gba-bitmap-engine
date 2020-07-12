//
// Created by Wouter Groeneveld on 28/07/18.
//

#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba/tonc_video.h>
#include <libgba-sprite-engine/renderer/gba_engine.h>
#include <cstring>
#include <libgba-sprite-engine/gba/tonc_core.h>
#include <libgba-sprite-engine/matrixfx.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/mesh.h>

std::unique_ptr<SoundControl> GBAEngine::activeChannelA;
std::unique_ptr<SoundControl> GBAEngine::activeChannelB;
std::unique_ptr<Timer> GBAEngine::timer;

u16 *vid_page;
int msecs = 1;

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
    showfps = true;
}

void GBAEngine::update() {
    // main update loop, in while(true) {}.
    // WARNING - keep amount of instructions as minimal as possible in here!

    u16 keys = readKeys();

    // TODO use software interrupt Vsyncing instead of 2 wasteful whiles
    vsync();
    renderClear();

    if(showfps) {
        showFPS();
    }

    currentScene->tick(keys);
    render();
    flipPage();
}

void GBAEngine::showFPS() {
    int curmsecs = getTimer()->getMsecs();
    int elapsed = curmsecs - msecs;
    int fps = 1000 / elapsed;
    msecs = curmsecs;

    TextStream::instance().setText(std::to_string(fps > 0 ? fps : 0) + std::string(" FPS"), 1, 1);
}

void GBAEngine::flipPage() {
    // toggle the write_buffer's page
    vid_page= (COLOR*)((u32)vid_page ^ VRAM_PAGE_SIZE);
    REG_DISPCNT ^= DCNT_PAGE;
}

void GBAEngine::plotPixel(int x, int y, u8 clrId) {
    m4_plot(x, y, clrId);
}

void GBAEngine::plotPixel(const VectorFx &pixel, u8 clrId) {
    plotPixel(fx2int(pixel.x()), fx2int(pixel.y()), clrId);
}

// uses tonc's optimalization tricks to get 10 FPS extra compared to standard bline algorithms
void GBAEngine::plotLine(int p0x, int p0y, int p1x, int p1y, u8 clrId) {
    m4_line(p0x, p0y, p1x, p1y, clrId);
}

void GBAEngine::plotLine(const VectorFx &point0, const VectorFx &point1, u8 clrId) {
    plotLine(fx2int(point0.x()), fx2int(point0.y()), fx2int(point1.x()), fx2int(point1.y()), clrId);
}

VectorFx GBAEngine::project(const VectorFx &coord, const MatrixFx &transMat) {
    auto point = MatrixFx::transformCoordinates(coord, transMat);

    auto x = fxmul(point.x(), GBA_SCREEN_WIDTH_FX) + fxdiv(GBA_SCREEN_WIDTH_FX, TWO);
    auto y = fxmul(-point.y(), GBA_SCREEN_HEIGHT_FX) + fxdiv(GBA_SCREEN_HEIGHT_FX, TWO);
    return VectorFx(x, y, point.z());
}

// does the mesh rendering - to write mem before flipping
void GBAEngine::render() {
    auto viewMatrix = MatrixFx::lookAtLH(currentCamera.getPosition(), currentCamera.getTarget(), VectorFx::up());

    for(auto& mesh :currentScene->meshes()) {
        auto worldMatrix = MatrixFx::rotationYawPitchRoll(mesh->roty(), mesh->rotx(), mesh->rotz()) * MatrixFx::translation(mesh->position());
        auto transformMatrix = worldMatrix * viewMatrix * projectionMatrix;

        renderer->render(transformMatrix, mesh);
    }
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
    getTimer()->start();

    auto fgPalette = scene->getForegroundPalette();
    fgPalette->persist();
    auto bgPalette = scene->getBackgroundPalette();
    bgPalette->persist();

    this->currentScene = scene;
    this->currentCamera = this->currentScene->camera();
}
