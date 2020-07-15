//
// Created by Wouter Groeneveld on 06/12/18.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_TIMER_H
#define GBA_SPRITE_ENGINE_PROJECT_TIMER_H

#include <ostream>

class Timer {
private:
    int microsecs, msecs, secs, minutes, hours;
    bool active;

public:

    Timer() : active(false) {
        reset();
    }

    void reset();
    void start();
    void toggle() {
        if(isActive()) stop();
        else start();
    }
    bool isActive() { return active; }
    void stop();
    void onvblank();

    std::string to_string();

    int getTotalMsecs();
    inline int getMsecs() { return msecs; }
    inline int getSecs() { return secs; }
    inline int getMinutes() { return minutes; }
    inline int getHours() { return hours; }

    friend std::ostream& operator<<(std::ostream& os, Timer& timer);
};

#endif //GBA_SPRITE_ENGINE_PROJECT_TIMER_H
