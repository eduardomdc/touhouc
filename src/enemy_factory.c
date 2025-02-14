#include "enemy_factory.h"
#include "firing_patterns.h"
#include "timer.h"

Enemy makeEnemy(Vector2 pos){
    Enemy enemy;
    enemy.pos = pos;
    enemy.radius = 4;
    enemy.alive = true;
    enemy.color = RED;
    Vector2 dir = {0, 1};
    enemy.fireDirection = dir;
    enemy.firePattern = fireShower;
    enemy.fireTimer = createTimer(0.01);
    enemy.bulletSpeed = 200;
    return enemy;
}