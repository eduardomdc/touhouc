#include "enemy.h"

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