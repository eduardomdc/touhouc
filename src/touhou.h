#ifndef TOUHOU_H
#define TOUHOU_H

static const int vRes = 700;
static const int hRes = 500;
static const int fps = 60;

void setupGame();
void renderGame();
void updateGame();
bool onScreen(Vector2 pos, float radius);

extern bool gameOver;

#endif