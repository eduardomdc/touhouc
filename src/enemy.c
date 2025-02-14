#include "enemy.h"

void renderEnemy(Enemy enemy){
    DrawCircleV(enemy.pos, enemy.radius, enemy.color);
}