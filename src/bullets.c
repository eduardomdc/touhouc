#include "bullets.h"
#include <raymath.h>
#include <stdio.h>

Bullet enemyBullets[MAX_ENEMY_BULLETS] = {0};
Bullet playerBullets[MAX_PLAYER_BULLETS] = {0};
BulletList enemyBulletList = {enemyBullets, MAX_ENEMY_BULLETS, ORANGE};
BulletList playerBulletList = {playerBullets, MAX_PLAYER_BULLETS, BLUE};

void renderBulletList(BulletList bulletList){
    for (int i=0; i < bulletList.len; i++){
        Bullet bullet = bulletList.bullets[i];
        if (bullet.active){
            DrawCircleV(bullet.pos, bullet.radius, bulletList.bulletColor);
        }
    }
}

void updateBulletList(BulletList* bulletList){
    float deltat = GetFrameTime();
    for (int i=0; i < bulletList->len; i++){
        Bullet bullet = bulletList->bullets[i];
        if (bullet.active){
            Vector2 moved = Vector2Scale(bullet.vel, deltat);
            bullet.pos = Vector2Add(bullet.pos, moved);
        }
        bulletList->bullets[i] = bullet;
    }
}

int addBulletToList(Bullet bullet, BulletList* bulletList){
    // looks for first slot avaible, returns 0 on success, 1 on failure
    for (int i=0; i < bulletList->len; i++){
        Bullet thisBullet = bulletList->bullets[i];
        if (!thisBullet.active){
            printf("bullet added to %d\n",i);
            bulletList->bullets[i] = bullet;
            return 0;
        }
    }
    return 1;
}