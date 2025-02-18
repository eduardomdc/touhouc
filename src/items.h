#ifndef ITEMS_H
#define ITEMS_H

#define MAX_ITEMS 30

#define ITEM_FALL_ACC 20

#include <raylib.h>
#include "assets.h"
#include "player.h"
#include "compact_array.h"

typedef enum {
    GAIN_POINT,
    GAIN_LIFE,
    INCREASE_FIRE_RATE,
    ITEM_EFFECTS_LEN,
} ItemEffects;

void itemEffectGainPoint(Player* player);
void itemEffectGainLife(Player* player);
void itemEffectIncreaseFireRate(Player* player);

extern void (*itemEffects[ITEM_EFFECTS_LEN])(Player*);

typedef struct Item {
    Vector2 pos;
    float speed;
    float radius;
    ItemEffects effect;
    ItemSprites sprite;
    SoundEffects pickupSound;
} Item;

void renderItems();
void updateItems();
void physicsUpdateItems(float dt);

void makePointItem(Vector2 pos);
void makePowerUpItem(Vector2 pos);
void makeOneUpItem(Vector2 pos);

extern Item items[MAX_ITEMS];
extern CompactArray itemsCArray;

#endif