#include "bullets.h"
#include "touhou.h"
#include "enemy.h"
#include "spawner.h"
#include "player.h"
#include <raymath.h>
#include <stdio.h>

Bullet enemyBullets[MAX_ENEMY_BULLETS] = {0};
Bullet playerBullets[MAX_PLAYER_BULLETS] = {0};
BulletList enemyBulletList = {enemyBullets, MAX_ENEMY_BULLETS, 0, GREEN, &enemyBulletSprite};
BulletList playerBulletList = {playerBullets, MAX_PLAYER_BULLETS, 0, RED, &playerBulletSprite};

void renderBulletList(BulletList bulletList){
    for (int i=0; i < bulletList.freeSlot; i++){
        Bullet bullet = bulletList.bullets[i];
        renderSpriteCentered(bulletList.sprite, bullet.pos);
        //DrawCircleV(bullet.pos, bullet.radius, bulletList.debugColor);
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
    for (int i=0; i < bulletList->freeSlot; i++){
        Bullet bullet = bulletList->bullets[i];
        bullet = moveBullet(bullet, deltaTime);
        Enemy* enemyHit = checkCollisionWithEnemy(bullet);
        if (enemyHit != NULL){
            enemyHit->alive = false;
            removeBulletFromList(i, bulletList);
            continue;
        }
        if (!onScreen(bullet.pos, bullet.radius)){
            removeBulletFromList(i, bulletList);
            continue;
        }
        bulletList->bullets[i] = bullet;
    }
}

void updateEnemyBulletList(){
    BulletList* bulletList = &enemyBulletList;
    float deltaTime = GetFrameTime();
    for (int i=0; i < bulletList->freeSlot; i++){
        Bullet bullet = bulletList->bullets[i];
        bullet = moveBullet(bullet, deltaTime);
        if (checkCollisionWithPlayer(bullet)){
            playerGetHit();
            removeBulletFromList(i, bulletList);
            continue;
        }
        if (!onScreen(bullet.pos, bullet.radius)){
            removeBulletFromList(i, bulletList);
            continue;
        }
        bulletList->bullets[i] = bullet;
    }
}

int addBulletToList(Bullet bullet, BulletList* bulletList){
    if (bulletList->freeSlot >= bulletList->len) return 1; // full list
    bulletList->bullets[bulletList->freeSlot] = bullet;
    bulletList->freeSlot++;
    return 0;
}

void removeBulletFromList(unsigned int index, BulletList* bulletList){
    Bullet* bullets = bulletList->bullets;
    if (index == bulletList->freeSlot-1){ // if index is last item
        if (bulletList->freeSlot > 0){
            bulletList->freeSlot--;
        }
        return;
    }
    bullets[index] = bullets[bulletList->freeSlot-1];
    if (bulletList->freeSlot > 0){
        bulletList->freeSlot--;
    }
}