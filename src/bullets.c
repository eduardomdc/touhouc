#include "bullets.h"
#include "touhou.h"
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

Bullet moveBullet(Bullet bullet, float deltaTime){
    Vector2 moved = Vector2Scale(bullet.direction, bullet.speed*deltaTime);
    bullet.pos = Vector2Add(bullet.pos, moved);
    return bullet;
}

void updateBulletList(BulletList* bulletList){
    float deltaTime = GetFrameTime();
    for (int i=0; i < bulletList->len; i++){
        Bullet bullet = bulletList->bullets[i];
        if (bullet.active){
            bullet = moveBullet(bullet, deltaTime);
        }
        if (!onScreen(bullet.pos, bullet.radius)){
            bullet.active = false;
        }
        bulletList->bullets[i] = bullet;
    }
}

int addBulletToList(Bullet bullet, BulletList* bulletList){
    // looks for first slot avaible, returns 0 on success, 1 on failure
    for (int i=0; i < bulletList->len; i++){
        Bullet thisBullet = bulletList->bullets[i];
        if (!thisBullet.active){
            bulletList->bullets[i] = bullet;
            return 0;
        }
    }
    return 1;
}