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

void enemyGetHit(int idx, int damage){
    Enemy* enemy = &enemyList[idx];
    enemy->hp -= damage;
    PlaySound(assets.soundEffects[SFX_ENEMY_HIT]);
    sendUDPSfx(SFX_ENEMY_HIT);
    if (enemy->hp <= 0){
        enemyDie(idx);
    }
}

void enemyDie(int idx){
    Enemy* enemy = &enemyList[idx];
    enemy->alive = false;
    makeItem(SCORE_ITEM, enemy->pos);
    compactRemoveItem(&compactEnemyArray, idx);
    //sendTcpEnemyDeath();
    PlaySound(assets.soundEffects[SFX_ENEMY_DEATH]);
    sendUDPSfx(SFX_ENEMY_DEATH);
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

EnemyData enemyData = {
    .angel = {.radius = 8, .alive = true, .sprite = SPRITE_ANGEL, .fireDirection = {0,1}, .bulletSpeed = 100, .bulletSprite = FIRE_BALL_8, .bulletRadius = 8, .moveSpeed = 100, .hp = 50, .sm = {.machine = SM_TYPE_ANGEL}},
    .jiangshi = {.radius = 8, .alive = true, .sprite = SPRITE_JIANGSHI, .fireDirection = {0,1}, .bulletSpeed = 150, .bulletSprite = BLUE_FIRE_15, .bulletRadius = 15, .moveSpeed = 300, .hp = 100, .sm = {.machine = SM_TYPE_JIANGSHI}}
};
