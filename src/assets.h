#ifndef ASSETS_H
#define ASSETS_H

#include "sprite.h"

typedef enum {
    FIRE_BALL_8,
    FIRE_BALL_12,
    BLUE_ARROW_8,
    BULLET_SPRITE_LEN
} BulletSprites;

typedef enum {
    MARISA,
    PLAYER_SPRITE_LEN
} PlayerSprites;

typedef enum {
    ANGEL,
    ENEMY_SPRITE_LEN
} EnemySprites;

typedef enum {
    CHAPEL,
    BACKGROUND_SPRITE_LEN
} BackgroundSprites;

typedef struct {
    Sprite bulletSprites[BULLET_SPRITE_LEN];
    Sprite playerSprites[PLAYER_SPRITE_LEN];
    Sprite enemySprites[ENEMY_SPRITE_LEN];
    Sprite backgroundSprites[BACKGROUND_SPRITE_LEN];
} Assets;

void loadAssets();

extern Assets assets;

#endif