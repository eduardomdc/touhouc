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

void renderSprite(Sprite* sprite, Vector2 pos){
    DrawTexture(sprite->tex, pos.x, pos.y, WHITE);
}

void renderSpriteCentered(Sprite* sprite, Vector2 pos){
    DrawTexture(sprite->tex, pos.x-sprite->center.x, pos.y-sprite->center.y, WHITE);
}

void renderSpriteCenteredOriented(Sprite* sprite, Vector2 pos, float angle){
    Vector2 corner;
    corner.x = pos.x-sprite->center.x;
    corner.y = pos.y-sprite->center.y;
    DrawTextureEx(sprite->tex, corner, angle, 1.0, WHITE);
}