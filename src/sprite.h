#ifndef SPRITE_H
#define SPRITE_H

#include <raylib.h>

typedef struct Sprite {
    Texture2D tex;
    Vector2 center;
} Sprite;

Sprite loadSprite(const char* fileName);
void renderSpriteCentered(Sprite* sprite, Vector2 pos);
void renderSpriteCenteredOriented(Sprite* sprite, Vector2 pos, float angle);

#endif