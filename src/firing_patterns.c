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
    Bullet bullet;
    bullet.pos = enemy->pos;
    bullet.radius = enemy->bulletRadius;
    enemy->fireDirection = Vector2Rotate(enemy->fireDirection, PI/5);
    bullet.direction = enemy->fireDirection;
    bullet.speed = enemy->bulletSpeed;
    bullet.sprite = FIRE_BALL_8;
    compactAddItem(&compactEnemyBulletArray, &bullet);
}
