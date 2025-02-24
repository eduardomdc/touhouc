#include "enemy.h"
#include "items.h"
#include "networking/packets.h"
#include <stdio.h>

void renderEnemy(Enemy enemy){
    renderSpriteCentered(&assets.enemySprites[enemy.sprite], enemy.pos);
}

void updateEnemy(Enemy* enemy){
    stateMachine[enemy->sm.machine](enemy);
}

void enemyDie(Enemy* enemy){
    makePointItem(enemy->pos);
    sendTcpEnemyDeath();
}
