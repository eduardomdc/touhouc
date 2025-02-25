#include "enemy.h"
#include "items.h"
#include "networking/packets.h"

void renderEnemy(Enemy enemy){
    renderSpriteCentered(&assets.enemySprites[enemy.sprite], enemy.pos);
}

void updateEnemy(Enemy* enemy){
}

void enemyDie(Enemy* enemy){
    makePointItem(enemy->pos);
    sendTcpEnemyDeath();
}
