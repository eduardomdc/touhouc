#include "ai.h"
#include "enemy.h"
#include "firing_patterns.h"
#include "timer.h"
#include "touhou.h"
#include <raylib.h>
#include <raymath.h>

void (*stateMachine[SM_TYPE_LEN])(void*) = {
    angelStateMachine,
    jiangshiStateMachine,
};

void (*angelState[ANGEL_STATE_LEN])(void*) = {
    angelStateInitial,
    angelStateMove,
    angelStateFire,
};

void (*jiangshiState[JIANGSHI_STATE_LEN])(void*) = {
    jiangshiStateInitial,
    jiangshiStateMove,
    jiangshiStateSideStep,
    jiangshiStateFire,
};

// ANGEL STATE MACHINE
void angelStateMachine(void *enemy){
    Enemy* angel = (Enemy*) enemy;
    angelState[angel->sm.state](enemy);
}

void angelStateInitial(void *enemy){
    // goes to MOVE state
    // choose a random target position to move to
    Enemy* angel = (Enemy*) enemy;
    Vector2 target;
    target.x = GetRandomValue(10, hRes-10);
    target.y = GetRandomValue(10, vRes/3);
    angel->sm.targetPos = target;
    angel->sm.speed = 100;
    angel->sm.state = ANGEL_STATE_MOVE;
}

void angelStateMove(void *enemy){
    // will move to target, then goes to FIRE
    Enemy* angel = (Enemy*) enemy;
    if (Vector2DistanceSqr(angel->pos, angel->sm.targetPos) < 0.1){
        angel->sm.firingTimer = createTimer(0.05);
        angel->sm.stateTimer = createTimer(1.1);
        angel->sm.state = ANGEL_STATE_FIRE;
    } else {
        angel->pos = Vector2MoveTowards(angel->pos, angel->sm.targetPos, angel->moveSpeed*GetFrameTime());
    }
}

void angelStateFire(void *enemy){
    // will fire until state timer expires, then goes back to INITIAL
    Enemy* angel = (Enemy*) enemy;
    updateTimer(&angel->sm.firingTimer);
    if (angel->sm.firingTimer.ready){
        firingPatterns[FIRING_SPIRAL](enemy);
        resetTimer(&angel->sm.firingTimer);
    }

    updateTimer(&angel->sm.stateTimer);
    if (angel->sm.stateTimer.ready){
        angel->sm.state = ANGEL_STATE_INITIAL;
    }
}

// JIANGSHI STATE MACHINE
void jiangshiStateMachine(void *enemy){
    Enemy* jiangshi = (Enemy*) enemy;
    jiangshiState[jiangshi->sm.state](enemy);
}

void jiangshiStateInitial(void *enemy){
    Enemy* jiangshi = (Enemy*) enemy;
    Vector2 target;
    target.x = GetRandomValue(10, hRes-10);
    target.y = GetRandomValue(10, vRes/3);
    jiangshi->sm.targetPos = target;
    jiangshi->sm.state = JIANGSHI_STATE_MOVE;
}

void jiangshiStateMove(void *enemy){
    Enemy* jiangshi = (Enemy*) enemy;
    if (Vector2DistanceSqr(jiangshi->pos, jiangshi->sm.targetPos) < 0.1){
        jiangshi->sm.firingTimer = createTimer(0.2);
        jiangshi->sm.stateTimer = createTimer(1);
        jiangshi->sm.state = JIANGSHI_STATE_FIRE;
    } else {
        jiangshi->pos = Vector2MoveTowards(jiangshi->pos, jiangshi->sm.targetPos, jiangshi->moveSpeed*GetFrameTime());
    }
}

void jiangshiStateFire(void* enemy){
    Enemy* jiangshi = (Enemy*) enemy;
    updateTimer(&jiangshi->sm.firingTimer);
    if (jiangshi->sm.firingTimer.ready){
        firingPatterns[FIRING_AT_PLAYER](enemy);
        resetTimer(&jiangshi->sm.firingTimer);
    }

    updateTimer(&jiangshi->sm.stateTimer);
    if (jiangshi->sm.stateTimer.ready){
        jiangshi->sm.state = JIANGSHI_STATE_SIDE_STEP;
    }
}

void jiangshiStateSideStep(void* enemy){
    Enemy* jiangshi = (Enemy*) enemy;
    Vector2 target;
    target.x = Clamp(jiangshi->pos.x + GetRandomValue(-200, 200), 10, hRes-10);
    target.y = jiangshi->pos.y;
    jiangshi->sm.targetPos = target;
    jiangshi->sm.state = JIANGSHI_STATE_MOVE;
    return;
}
