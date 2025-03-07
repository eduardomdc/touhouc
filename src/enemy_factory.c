#include "enemy_factory.h"
#include "ai.h"
#include "assets.h"

EnemyData enemyData = {
    .angel = {.radius = 8, .alive = true, .sprite = SPRITE_ANGEL, .fireDirection = {0,1}, .bulletSpeed = 100, .bulletSprite = FIRE_BALL_8, .bulletRadius = 8, .moveSpeed = 100, .hp = 50, .sm = {.machine = SM_TYPE_ANGEL}},
    .jiangshi = {.radius = 8, .alive = true, .sprite = SPRITE_JIANGSHI, .fireDirection = {0,1}, .bulletSpeed = 150, .bulletSprite = BLUE_FIRE_15, .bulletRadius = 15, .moveSpeed = 300, .hp = 100, .sm = {.machine = SM_TYPE_JIANGSHI}}
};
