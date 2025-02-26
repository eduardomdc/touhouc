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
    EnemySprites sprite;
    bool alive;
} Enemy;

extern Sprite enemySprite;

void renderEnemy(Enemy enemy);
void updateEnemy(Enemy* enemy);
void enemyDie(int idx);

#endif
