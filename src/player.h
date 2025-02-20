#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include "timer.h"
#include "sprite.h"
#include "assets.h"

static const int playerSize = 5;
static const float playerSpeed = 200;
static const Color playerColor = BLUE;

void setupPlayer();
void renderPlayer();
void updatePlayer();
void playerFire();
void handleInput();
void movePlayer(Vector2 inputDir);
void playerSetFireRate(float fireRate);
void playerGetHit();

typedef struct Player {
    Vector2 pos;
    int lifes;
    unsigned int points;
    float bulletSpeed;
    float bulletRadius;
    float bulletSpreadAngle;
    float fireRate;
    PlayerSprites sprite;
    Timer fireTimer;
    bool alive;
} Player;

void playerPickUp(short itemType, Player* player);
void playerSetFireRate(float fireRate);

extern Player player;

#endif
