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
    sendTcpEnemyDeath();
}
