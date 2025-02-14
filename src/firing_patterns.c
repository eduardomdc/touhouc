#include <raylib.h>
#include <raymath.h>
#include "firing_patterns.h"
#include "bullets.h"
#include "enemy.h"

void fireShower(void* enemyPtr){
    Enemy* enemy = (Enemy*) enemyPtr;
    Bullet bullet;
    bullet.pos = enemy->pos;
    bullet.active = true;
    bullet.radius = enemy->bulletRadius;
    enemy->fireDirection = Vector2Rotate(enemy->fireDirection, 0.4);
    bullet.direction = enemy->fireDirection;
    bullet.speed = enemy->bulletSpeed;
    addBulletToList(bullet, &enemyBulletList);
}