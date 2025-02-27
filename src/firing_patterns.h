#ifndef FIRING_PATTERNS_H
#define FIRING_PATTERNS_H

typedef enum {
    FIRING_SPIRAL,
    FIRING_AT_PLAYER,
    FIRING_PATTERNS_LEN
} FiringPatterns;

void fireSpiral(void* enemy);
void fireAtPlayer(void* enemy);

extern void (*firingPatterns[FIRING_PATTERNS_LEN])(void*);

#endif
