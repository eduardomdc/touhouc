#ifndef FIRING_PATTERNS_H
#define FIRING_PATTERNS_H
#include "enemy.h"

typedef enum {
    SPIRAL,
    FIRING_PATTERNS_LEN
} FiringPatterns;

void fireSpiral(void* enemy);

extern void (*firingPatterns[FIRING_PATTERNS_LEN])(void*);

#endif