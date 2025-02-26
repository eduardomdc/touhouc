#ifndef BULLETS_H
#define BULLETS_H

#include <raylib.h>
#include "sprite.h"
#include "compact_array.h"
#include "assets.h"
#include "enemy.h"

#define MAX_ENEMY_BULLETS 1000
#define MAX_PLAYER_BULLETS 100

typedef enum {
    TEAM_PLAYERS,
    TEAM_ENEMY,
} Team;

typedef struct Bullet {
    Vector2 pos;
    Vector2 direction;
    float speed;
    float radius;
    BulletSprites sprite;
} Bullet;

void renderBulletCArray(CompactArray compactBulletArray);
void physicsUpdateBullets(CompactArray* bulletCArray, float dt);
void updatePlayerBullets();
void updateEnemyBullets();

extern Bullet enemyBullets[MAX_ENEMY_BULLETS];
extern Bullet playerBullets[MAX_PLAYER_BULLETS];
extern CompactArray compactEnemyBulletArray;
extern CompactArray compactPlayerBulletArray;

#endif
