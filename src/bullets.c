#include "bullets.h"
#include "touhou.h"
#include "enemy.h"
#include "spawner.h"
#include "player.h"
#include <raymath.h>
#include <stdio.h>

Bullet enemyBullets[MAX_ENEMY_BULLETS] = {0};
Bullet playerBullets[MAX_PLAYER_BULLETS] = {0};
CompactArray compactEnemyBulletArray = {enemyBullets, sizeof(Bullet), MAX_ENEMY_BULLETS, 0};
CompactArray compactPlayerBulletArray = {playerBullets, sizeof(Bullet), MAX_PLAYER_BULLETS, 0};

void renderBulletCArray(CompactArray compactBulletArray){
    Bullet* bullets = (Bullet*) compactBulletArray.array;
    for (int i=0; i < compactBulletArray.freeIndex; i++){
        Bullet bullet = bullets[i];
        renderSpriteCentered(&assets.bulletSprites[bullet.sprite], bullet.pos);
        //DrawCircleV(bullet.pos, bullet.radius, RED);
    }
}

Bullet moveBullet(Bullet bullet, float deltaTime){
    Vector2 moved = Vector2Scale(bullet.direction, bullet.speed*deltaTime);
    bullet.pos = Vector2Add(bullet.pos, moved);
    return bullet;
}

int checkCollisionWithPlayer(Bullet bullet){
    for (int i = 0; i < PLAYER_CHARACTER_LEN; i++){
        Player player = players[i];
        float distance = Vector2Distance(bullet.pos, player.pos);
        if (distance < (bullet.radius + playerSize) && player.connected && player.alive){
            return i;
        }
    }
    return -1;
}

int checkCollisionWithEnemy(Bullet bullet){
    // returns index of collided enemy, if no collision returns -1
    for (int i=0; i < compactEnemyArray.freeIndex; i++){
        Enemy* enemy = &enemyList[i];
        if (enemy->alive){
            float distance = Vector2Distance(bullet.pos, enemy->pos); // rewrite to use squared distance for perfomance
            if (distance < (bullet.radius + enemy->radius)){
                return i;
            }
        }
    }
    return -1;
}

void physicsUpdateBullets(CompactArray* bulletCArray, float dt){
    Bullet* bullets = (Bullet*) bulletCArray->array;
    for (int i=0; i < bulletCArray->freeIndex; i++){
        Bullet bullet = bullets[i];
        bullet = moveBullet(bullet, dt);
        bullets[i] = bullet;
    }
}

void updatePlayerBullets(){
    CompactArray* bulletCArray = &compactPlayerBulletArray;
    Bullet* bullets = (Bullet*) bulletCArray->array;
    for (int i=0; i < bulletCArray->freeIndex; i++){
        Bullet bullet = bullets[i];
        int enemyHit = checkCollisionWithEnemy(bullet);
        if (enemyHit != -1){
            enemyGetHit(enemyHit, players[bullet.player].damage);
            compactRemoveItem(bulletCArray, i);
            continue;
        }
        if (!onScreen(bullet.pos, bullet.radius)){
            compactRemoveItem(bulletCArray, i);
            continue;
        }
        bullets[i] = bullet;
    }
}

void updateEnemyBullets(){
    CompactArray* bulletCArray = &compactEnemyBulletArray;
    Bullet* bullets = (Bullet*) bulletCArray->array;
    for (int i=0; i < bulletCArray->freeIndex; i++){
        Bullet bullet = bullets[i];
        int player;
        player = checkCollisionWithPlayer(bullet);
        if (player >= 0){
            playerGetHit(&players[player]);
            compactRemoveItem(bulletCArray, i);
            continue;
        }
        if (!onScreen(bullet.pos, bullet.radius)){
            compactRemoveItem(bulletCArray, i);
            continue;
        }
        bullets[i] = bullet;
    }
}
