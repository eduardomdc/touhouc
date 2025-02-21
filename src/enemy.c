#include "enemy.h"
#include "items.h"
#include "networking/packets.h"

void renderEnemy(Enemy enemy){
    renderSpriteCentered(&assets.enemySprites[enemy.sprite], enemy.pos);
}

void updateEnemy(Enemy* enemy){
    updateTimer(&enemy->fireTimer);
    if (enemy->fireTimer.ready){
        enemy->firePattern(enemy);
        resetTimer(&enemy->fireTimer);
    }
}

void enemyDie(Enemy* enemy){
    makePointItem(enemy->pos);
    sendTcpEnemyDeath();
}