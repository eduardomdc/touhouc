#include "enemy.h"

void renderEnemy(Enemy enemy){
    renderSpriteCentered(enemy.sprite, enemy.pos);
    //DrawCircleV(enemy.pos, enemy.radius, enemy.color);
}

void updateEnemy(Enemy* enemy){
    updateTimer(&enemy->fireTimer);
    if (enemy->fireTimer.ready){
        enemy->firePattern(enemy);
        resetTimer(&enemy->fireTimer);
    }
}