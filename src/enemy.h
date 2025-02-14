#ifndef ENEMY_H
#define ENEMY_H

#include <raylib.h>
#include "timer.h"
#include "sprite.h"

typedef struct Enemy {
    Vector2 pos;
    Vector2 fireDirection;
    float radius;
    float bulletSpeed;
    float bulletRadius;
    Timer fireTimer;
    void (*firePattern)(void* enemy);
    Color color; // future: texture pointer
    Sprite* sprite;
    bool alive;
} Enemy;

extern Sprite enemySprite;

void renderEnemy(Enemy enemy);
void updateEnemy(Enemy* enemy);

#endif