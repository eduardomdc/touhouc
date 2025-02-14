#ifndef TOUHOU_H
#define TOUHOU_H

#include "sprite.h"

static const int vRes = 650;
static const int hRes = 500;
static const int fps = 120;

void setupGame();
void loadSprites();
void renderGame();
void updateGame();
void updateEnemies();
bool onScreen(Vector2 pos, float radius);

extern bool gameOver;
extern Texture2D backgroundTexture;
extern Sprite enemyBulletSprite;
extern Sprite playerBulletSprite;

#endif