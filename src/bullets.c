#include "bullets.h"
#include "touhou.h"
#include "enemy.h"
#include "spawner.h"
#include "player.h"
#include <raymath.h>
#include <stdio.h>

Bullet enemyBullets[MAX_ENEMY_BULLETS] = {0};
Bullet playerBullets[MAX_PLAYER_BULLETS] = {0};
BulletList enemyBulletList = {enemyBullets, MAX_ENEMY_BULLETS, ORANGE, ENEMY};
BulletList playerBulletList = {playerBullets, MAX_PLAYER_BULLETS, BLUE, PLAYER};

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

bool checkCollisionWithPlayer(Bullet bullet){
    float distance = Vector2Distance(bullet.pos, player.pos);
    if (distance < (bullet.radius + playerSize)){
        return true;
    }
    return false;
}

Enemy* checkCollisionWithEnemy(Bullet bullet){
    for (int i=0; i < MAX_ENEMIES; i++){
        Enemy* enemy = &enemyList[i];
        if (enemy->alive){
            float distance = Vector2Distance(bullet.pos, enemy->pos); // rewrite to use squared distance for perfomance
            if (distance < (bullet.radius + enemy->radius)){
                return enemy;
            }
        }
    }
    return NULL;
}

void updatePlayerBulletList(){
    BulletList* bulletList = &playerBulletList;
    float deltaTime = GetFrameTime();
    for (int i=0; i < bulletList->len; i++){
        Bullet bullet = bulletList->bullets[i];
        if (bullet.active){
            bullet = moveBullet(bullet, deltaTime);
            Enemy* enemyHit = checkCollisionWithEnemy(bullet);
            if (enemyHit != NULL){
                enemyHit->alive = false;
                bullet.active = false;
                bulletList->bullets[i] = bullet;
                continue;
            }
        }
        if (!onScreen(bullet.pos, bullet.radius)){
            bullet.active = false;
        }
        bulletList->bullets[i] = bullet;
    }
}

void updateEnemyBulletList(){
    BulletList* bulletList = &enemyBulletList;
    float deltaTime = GetFrameTime();
    for (int i=0; i < bulletList->len; i++){
        Bullet bullet = bulletList->bullets[i];
        if (bullet.active){
            bullet = moveBullet(bullet, deltaTime);
            if (checkCollisionWithPlayer(bullet)){
                playerGetHit();
                bullet.active = false;
                bulletList->bullets[i] = bullet;
                continue;
            }
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