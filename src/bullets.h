#ifndef BULLETS_H
#define BULLETS_H

#include <raylib.h>

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
    bool active;
} Bullet;

typedef struct BulletList {
    Bullet* bullets;
    int len;
    Color bulletColor; // texture in the future
    enum Team team;
} BulletList;


void renderBulletList(BulletList bulletList);
void updateBulletList(BulletList* bulletList);
int addBulletToList(Bullet bullet, BulletList* bulletList);

extern Bullet enemyBullets[MAX_ENEMY_BULLETS];
extern Bullet playerBullets[MAX_PLAYER_BULLETS];
extern BulletList enemyBulletList;
extern BulletList playerBulletList;

#endif