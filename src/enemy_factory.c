#include "enemy_factory.h"
#include "ai.h"

Enemy makeEnemy(Vector2 pos){
    Enemy enemy;
    enemy.pos = pos;
    enemy.radius = 8;
    enemy.alive = true;
    enemy.sprite = ANGEL;
    Vector2 dir = {0, 1};
    enemy.fireDirection = dir;
    enemy.bulletSpeed = 100;
    enemy.bulletRadius = 8;
    enemy.sm = createStateMachine(SM_TYPE_ANGEL);
    return enemy;
}
