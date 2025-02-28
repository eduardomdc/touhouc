#include "enemy.h"
#include "compact_array.h"
#include "items.h"
#include "networking/packets.h"
#include "spawner.h"

void renderEnemy(Enemy enemy){
    renderSpriteCentered(&assets.enemySprites[enemy.sprite], enemy.pos);
}

void updateEnemy(Enemy* enemy){
    stateMachine[enemy->sm.machine](enemy);
}

void enemyDie(int idx){
    Enemy* enemy = &enemyList[idx];
    enemy->alive = false;
    makePointItem(enemy->pos);
    compactRemoveItem(&compactEnemyArray, idx);
    //sendTcpEnemyDeath();
    sendUDPSfx(ENEMY_HIT);
}

void enemyFire(Enemy* enemy){
    Bullet bullet;
    bullet.pos = enemy->pos;
    bullet.radius = enemy->bulletRadius;
    bullet.direction = enemy->fireDirection;
    bullet.speed = enemy->bulletSpeed;
    bullet.sprite = enemy->bulletSprite;
    compactAddItem(&compactEnemyBulletArray, &bullet);
}