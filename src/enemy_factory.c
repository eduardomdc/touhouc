#include "enemy_factory.h"
#include "ai.h"

Enemy makeAngel(Vector2 pos){
    Enemy enemy;
    enemy.pos = pos;
    enemy.radius = 8;
    enemy.alive = true;
    enemy.sprite = SPRITE_ANGEL;
    Vector2 dir = {0, 1};
    enemy.fireDirection = dir;
    enemy.bulletSpeed = 100;
    enemy.moveSpeed = 100;
    enemy.bulletRadius = 8;
    enemy.bulletSprite = FIRE_BALL_8;
    enemy.sm = createStateMachine(SM_TYPE_ANGEL);
    return enemy;
}

Enemy makeJiangshi(Vector2 pos){
    Enemy enemy;
    enemy.pos = pos;
    enemy.radius = 8;
    enemy.alive = true;
    enemy.sprite = SPRITE_JIANGSHI;
    Vector2 dir = {0, 1};
    enemy.fireDirection = dir;
    enemy.moveSpeed = 300;
    enemy.bulletSpeed = 150;
    enemy.bulletRadius = 15;
    enemy.bulletSprite = BLUE_FIRE_15;
    enemy.sm = createStateMachine(SM_TYPE_JIANGSHI);
    return enemy;
}