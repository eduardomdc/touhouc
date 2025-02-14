/*
Timer counts until 'duration' has passed
then ready is set to true.
*/
#include <raylib.h>

typedef struct Timer {
    double start;
    double duration;
    bool ready;
} Timer;

Timer createTimer(double duration);
void updateTimer(Timer* timer);
void resetTimer(Timer* timer);