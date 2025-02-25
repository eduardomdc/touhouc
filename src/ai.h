#ifndef AI_H
#define AI_H
#include <raylib.h>
#include "timer.h"

typedef enum {
    AI_TYPE_ANGEL,
    AI_TYPE_LEN
} AiType;

typedef enum {
    ANGEL_STATE_INITIAL,
    ANGEL_STATE_MOVE,
    ANGEL_STATE_FIRE,
    ANGEL_STATE_LEN
} AngelStates;

void angelStateInitial(void* enemy);
void angelStateMove(void* enemy);
void angelStateFire(void* enemy);
extern void (*angelStateUpdate[ANGEL_STATE_LEN])(void*);

typedef struct {
    char state;
    Vector2 targetPos;
    Timer stateTimer;
} StateMachine;

#endif
