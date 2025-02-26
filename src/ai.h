#ifndef AI_H
#define AI_H
#include <raylib.h>
#include "timer.h"

typedef enum {
    SM_TYPE_ANGEL,
    SM_TYPE_JIANGSHI,
    SM_TYPE_LEN
} SmType;

void angelStateMachine(void* enemy);
extern void (*stateMachine[SM_TYPE_LEN])(void*);

typedef enum {
    ANGEL_STATE_INITIAL,
    ANGEL_STATE_MOVE,
    ANGEL_STATE_FIRE,
    ANGEL_STATE_LEN
} AngelStates;

void angelStateInitial(void* enemy);
void angelStateMove(void* enemy);
void angelStateFire(void* enemy);
extern void (*angelState[ANGEL_STATE_LEN])(void*);

typedef struct {
    char state;
    char machine;
    float speed;
    Vector2 moveDir;
    Vector2 targetPos;
    Timer stateTimer;
    Timer firingTimer;
} StateMachine;

StateMachine createStateMachine(SmType machine);

#endif
