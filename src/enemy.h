#ifndef ENEMY_H
#define ENEMY_H

#include <raylib.h>
#include "ai.h"
#include "timer.h"
#include "sprite.h"
#include "assets.h"

typedef struct Enemy {
    StateMachine sm;
    Vector2 pos;
    Vector2 fireDirection;
    float radius;
    float bulletSpeed;
    float bulletRadius;
    float moveSpeed;
    int hp;
    EnemySprites sprite;
    BulletSprites bulletSprite;
    bool alive;
} Enemy;

extern Sprite enemySprite;

void renderEnemy(Enemy enemy);
void updateEnemy(Enemy* enemy);
void enemyGetHit(int idx, int damage);
void enemyDie(int idx);
void enemyFire(Enemy* enemy);

#endif
