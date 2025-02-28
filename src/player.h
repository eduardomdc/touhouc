#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include "timer.h"
#include "sprite.h"
#include "assets.h"
#include "input.h"

static const int playerSize = 5;
static const float playerSpeed = 200;
static const Color playerColor = BLUE;

typedef enum {
    MARISA,
    REIMU,
    PLAYER_CHARACTER_LEN
} PlayerCharacter;

typedef struct Player {
    Vector2 pos;
    int lives;
    unsigned int score;
    float bulletSpeed;
    float bulletRadius;
    float bulletSpreadAngle;
    float damage;
    float fireRate;
    PlayerSprites sprite;
    BulletSprites bulletSprite;
    PlayerCharacter character;
    Timer fireTimer;
    Timer invulnerableTimer;
    Color hitBoxColor;
    bool alive;
    bool invulnerable;
    bool connected;
} Player;

void setupPlayers();
void renderPlayers();
void updatePlayer(Player* player, Input input);
void playerFire(Player* player);
void movePlayer(Player* player, Vector2 inputDir);
void playerSetFireRate(Player* player, float fireRate);
void playerGetHit(Player* player);
void playerPickUp(short itemType, Player* player);

extern Player players[PLAYER_CHARACTER_LEN];

#endif
