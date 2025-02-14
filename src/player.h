#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include "timer.h"

static const int playerSize = 5;
static const float playerSpeed = 100;
static const Color playerColor = WHITE;

void setupPlayer();
void renderPlayer();
void updatePlayer();
void playerFire();

typedef struct Player {
    Vector2 pos;
    unsigned int lifes;
    float bulletSpeed;
    float fireRate;
    Timer fireTimer;
} Player;

extern Player player;

#endif