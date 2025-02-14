#include "sprite.h"

Sprite loadSprite(const char* fileName){
    Sprite sprite;
    sprite.tex = LoadTexture(fileName);
    Vector2 center;
    center.x = sprite.tex.width/2;
    center.y = sprite.tex.height/2;
    sprite.center = center;
    return sprite;
}

void renderSpriteCentered(Sprite* sprite, Vector2 pos){
    DrawTexture(sprite->tex, pos.x-sprite->center.x, pos.y-sprite->center.y, WHITE);
}