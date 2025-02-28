#ifndef TOUHOU_H
#define TOUHOU_H

#include "sprite.h"
#include "input.h"

#define MAX_SIZE_IP 39 // ipv6 has 39 characters

//#define DEBUG

static const int vRes = 650;
static const int hRes = 500;
static const int fps = 120;

void setupGame();
void loadSprites();
void renderGame();
Input handleInput();
void updateGame(Input input);
void updateEnemies();
bool onScreen(Vector2 pos, float radius);

extern bool gameOver;
extern bool isServer;
extern bool gameClosed;
extern char serverIpStr[MAX_SIZE_IP+1];

#endif