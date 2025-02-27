#ifndef TOUHOU_H
#define TOUHOU_H

#include "sprite.h"
#include "input.h"

static const int vRes = 650;
static const int hRes = 500;
static const int fps = 120;

void setupGame(char* hostType);
void loadSprites();
void renderGame();
Input handleInput();
void updateGame(Input input);
void updateEnemies();
bool onScreen(Vector2 pos, float radius);

extern bool gameOver;
extern bool isServer;
extern bool gameClosed;

#endif