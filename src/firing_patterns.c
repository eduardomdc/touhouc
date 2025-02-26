#include <raylib.h>
#include <raymath.h>
#include "firing_patterns.h"
#include "bullets.h"
#include "enemy.h"

void (*firingPatterns[FIRING_PATTERNS_LEN])(void*) = {
    fireSpiral
};

void fireSpiral(void* enemyPtr){
    Enemy* enemy = (Enemy*) enemyPtr;
    enemy->fireDirection = Vector2Rotate(enemy->fireDirection, PI/5);
    enemyFire(enemy);
}
