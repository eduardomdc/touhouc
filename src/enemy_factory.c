#include "enemy_factory.h"
#include "firing_patterns.h"
#include "timer.h"

Enemy makeEnemy(Vector2 pos){
    Enemy enemy;
    enemy.pos = pos;
    enemy.radius = 8;
    enemy.alive = true;
    enemy.sprite = ANGEL;
    Vector2 dir = {0, 1};
    enemy.fireDirection = dir;
    enemy.firePattern = firingPatterns[SPIRAL];
    enemy.fireTimer = createTimer(0.03);
    enemy.bulletSpeed = 100;
    enemy.bulletRadius = 8;
    return enemy;
}