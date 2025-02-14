#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

static const int playerSize = 5;
static const float playerSpeed = 100;
static const Color playerColor = WHITE;

void setupPlayer();
void renderPlayer();
void updatePlayer();

typedef struct Player {
    Vector2 pos;
    unsigned int lifes;
    float bulletSpeed;
} Player;

extern Player player;

#endif