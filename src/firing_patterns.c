#include <raylib.h>
#include <raymath.h>
#include "firing_patterns.h"
#include "enemy.h"
#include "player.h"

void (*firingPatterns[FIRING_PATTERNS_LEN])(void*) = {
    fireSpiral,
    fireAtPlayer,
};

void fireSpiral(void* enemyPtr){
    Enemy* enemy = (Enemy*) enemyPtr;
    enemy->fireDirection = Vector2Rotate(enemy->fireDirection, PI/5);
    enemyFire(enemy);
}

void fireAtPlayer(void* enemyPtr){
    Enemy* enemy = (Enemy*) enemyPtr;
    int randomPlayer;
    if (players[REIMU].alive && players[REIMU].connected && players[MARISA].alive && players[MARISA].connected){
        randomPlayer = GetRandomValue(MARISA, REIMU);
    } else if (players[MARISA].alive && players[MARISA].connected){
        randomPlayer = MARISA;
    } else if (players[REIMU].alive && players[REIMU].connected){
        randomPlayer = REIMU;
    } else {
        return;
    }

    Vector2 dir = Vector2Normalize(Vector2Subtract(players[randomPlayer].pos, enemy->pos));
    enemy->fireDirection = dir;
    enemyFire(enemy);
}
