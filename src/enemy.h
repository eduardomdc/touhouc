#ifndef ENEMY_H
#define ENEMY_H

#include <raylib.h>
#include "timer.h"

typedef struct Enemy {
    Vector2 pos;
    Vector2 fireDirection;
    Color color; // future: texture pointer
    float radius;
    Timer fireTimer;
    void (*firePattern)(void* enemy);
    bool alive;
} Enemy;

void renderEnemy(Enemy enemy);

#endif