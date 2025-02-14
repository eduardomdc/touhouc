#include "timer.h"

Timer createTimer(double duration){
    Timer timer;
    timer.start = GetTime();
    timer.duration = duration;
    timer.ready = false;
    return timer;
}

void updateTimer(Timer* timer){
    if (!timer->ready){
        double now = GetTime();
        if (now-timer->start > timer->duration){
            timer->ready = true;
        }
    }
}

void resetTimer(Timer* timer){
    timer->ready = false;
    timer->start = GetTime();
}