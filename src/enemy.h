#ifndef ENEMY_H
#define ENEMY_H

#include <raylib.h>
#include "timer.h"

typedef struct Enemy {
    Vector2 pos;
    Vector2 fireDirection;
    float radius;
    float bulletSpeed;
    Timer fireTimer;
    void (*firePattern)(void* enemy);
    Color color; // future: texture pointer
    bool alive;
} Enemy;

void renderEnemy(Enemy enemy);
void updateEnemy(Enemy* enemy);

#endif