#ifndef ASSETS_H
#define ASSETS_H

#include "sprite.h"

typedef enum {
    FIRE_BALL_8,
    FIRE_BALL_12,
    BLUE_ARROW_8,
    BULLET_SPRITES_LEN
} BulletSprites;

typedef enum {
    POINT_SPRITE,
    POWER_UP_SPRITE,
    ONE_UP_SPRITE,
    ITEM_SPRITES_LEN,
} ItemSprites;

typedef enum {
    MARISA,
    PLAYER_SPRITES_LEN
} PlayerSprites;

typedef enum {
    ANGEL,
    ENEMY_SPRITES_LEN
} EnemySprites;

typedef enum {
    CHAPEL,
    BACKGROUND_SPRITES_LEN
} BackgroundSprites;

typedef enum {
    PLAYER_FIRE,
    PLAYER_HIT,
    PLAYER_DEATH,
    ENEMY_HIT,
    POINT_PICKUP,
    POWERUP_PICKUP,
    SOUND_EFFECTS_LEN
} SoundEffects;

typedef enum {
    DESERTED_HELL,
    BGM_LEN
} BGM;

typedef struct {
    Sprite bulletSprites[BULLET_SPRITES_LEN];
    Sprite playerSprites[PLAYER_SPRITES_LEN];
    Sprite enemySprites[ENEMY_SPRITES_LEN];
    Sprite backgroundSprites[BACKGROUND_SPRITES_LEN];
    Sprite itemSprites[ITEM_SPRITES_LEN];
    Sound soundEffects[SOUND_EFFECTS_LEN];
    Music bgm[BGM_LEN];
} Assets;

void loadAssets();

extern Assets assets;

#endif