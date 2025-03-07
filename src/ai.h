#ifndef AI_H
#define AI_H
#include <raylib.h>
#include "timer.h"

typedef enum {
    SM_TYPE_ANGEL,
    SM_TYPE_JIANGSHI,
    SM_TYPE_LEN
} SmType;

typedef struct {
    char state;
    char machine;
    float speed;
    Vector2 moveDir;
    Vector2 targetPos;
    Timer stateTimer;
    Timer firingTimer;
} StateMachine;

void angelStateMachine(void* enemy);
void jiangshiStateMachine(void* enemy);
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

typedef enum {
    JIANGSHI_STATE_INITIAL,
    JIANGSHI_STATE_MOVE,
    JIANGSHI_STATE_SIDE_STEP,
    JIANGSHI_STATE_FIRE,
    JIANGSHI_STATE_LEN
} JiangshiStates;

void jiangshiStateInitial(void* enemy);
void jiangshiStateMove(void* enemy);
void jiangshiStateSideStep(void* enemy);
void jiangshiStateFire(void* enemy);
extern void (*jiangshiState[JIANGSHI_STATE_LEN])(void*);

#endif
