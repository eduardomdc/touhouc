#include "ai.h"
#include "enemy.h"
#include "firing_patterns.h"
#include "timer.h"
#include "touhou.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

void (*stateMachine[SM_TYPE_LEN])(void*) = {
    angelStateMachine
};

void (*angelState[ANGEL_STATE_LEN])(void*) = {
    angelStateInitial,
    angelStateMove,
    angelStateFire,
};

StateMachine createStateMachine(SmType machine){
    StateMachine sm = {0};
    sm.state = 0; // 0 is always initial state
    sm.machine = machine;
    return sm;
}


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
        angel->sm.firingTimer = createTimer(0.1);
        angel->sm.stateTimer = createTimer(1.0);
        angel->sm.state = ANGEL_STATE_FIRE;
    } else {
        angel->pos = Vector2MoveTowards(angel->pos, angel->sm.targetPos, angel->sm.speed);
    }
}

void angelStateFire(void *enemy){
    // will fire until state timer expires, then goes back to INITIAL
    Enemy* angel = (Enemy*) enemy;
    updateTimer(&angel->sm.firingTimer);
    if (angel->sm.firingTimer.ready){
        firingPatterns[SPIRAL](enemy);
        resetTimer(&angel->sm.firingTimer);
    }
    
    updateTimer(&angel->sm.stateTimer);
    if (angel->sm.stateTimer.ready){
        angel->sm.state = ANGEL_STATE_INITIAL;
    }
}
