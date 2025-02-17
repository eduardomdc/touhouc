#ifndef BULLETS_H
#define BULLETS_H

#include <raylib.h>
#include "sprite.h"

#define MAX_ENEMY_BULLETS 500
#define MAX_PLAYER_BULLETS 100

enum Team {
    PLAYER,
    ENEMY,
};

typedef struct Bullet {
    Vector2 pos;
    Vector2 direction;
    float speed;
    float radius;
} Bullet;

typedef struct BulletList {
    Bullet* bullets;
    int len;
    int freeSlot;
    Color debugColor;
    Sprite* sprite;
} BulletList;


void renderBulletList(BulletList bulletList);
void updatePlayerBulletList();
void updateEnemyBulletList();
int addBulletToList(Bullet bullet, BulletList* bulletList);
void removeBulletFromList(unsigned int index, BulletList* BulletList);

extern Bullet enemyBullets[MAX_ENEMY_BULLETS];
extern Bullet playerBullets[MAX_PLAYER_BULLETS];
extern BulletList enemyBulletList;
extern BulletList playerBulletList;

#endif