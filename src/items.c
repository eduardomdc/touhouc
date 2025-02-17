#include "items.h"
#include "touhou.h"
#include <raymath.h>

Item items[MAX_ITEMS] = {0};
CompactArray itemsCArray = {items, sizeof(Item), MAX_ITEMS, 0};

#include <stdio.h>
void renderItems(){
    for (int i=0; i < itemsCArray.freeIndex; i++){
        Item item = items[i];
        fprintf(stderr, "render item %d\n", i);
        renderSpriteCentered(&assets.itemSprites[item.sprite], item.pos);
    }
}

bool collidingWithPlayer(Item item){
    float distance = Vector2Distance(item.pos, player.pos);
    if (distance < (item.radius + playerSize)){
        return true;
    }
    return false;
}

Item moveItem(Item item, float deltaTime){
    Vector2 dir = {0, 1};
    Vector2 moved = Vector2Scale(dir, item.speed*deltaTime);
    item.pos = Vector2Add(item.pos, moved);
    return item;
}

void updateItems(Item* item){
    float deltaTime = GetFrameTime();
    for (int i=0; i < itemsCArray.freeIndex; i++){
        Item item = items[i];
        item.speed += ITEM_FALL_ACC*deltaTime;
        item = moveItem(item, deltaTime);
        if (collidingWithPlayer(item) && player.alive){
            PlaySound(assets.soundEffects[item.pickupSound]);
            itemEffects[item.effect](&player);
            compactRemoveItem(&itemsCArray, i);
            continue;
        }
        if (!onScreen(item.pos, item.radius)){
            compactRemoveItem(&itemsCArray, i);
            continue;
        }
        items[i] = item;
    }
}

void (*itemEffects[ITEM_EFFECTS_LEN])(Player*) = {
    itemEffectGainPoint,
    itemEffectGainLife,
    itemEffectIncreaseFireRate,
};
void itemEffectGainPoint(Player* player) {
    player->points++;
}
void itemEffectGainLife(Player* player) {
    player->lifes++;
}
void itemEffectIncreaseFireRate(Player* player) {
    playerSetFireRate(player->fireRate*2);
}

void makePointItem(Vector2 pos){
    Item item;
    item.pos = pos;
    item.effect = GAIN_POINT;
    item.pickupSound = POINT_PICKUP;
    item.radius = 6;
    item.speed = 0;
    item.sprite = POINT_SPRITE;
    compactAddItem(&itemsCArray, &item);
}

void makePowerUpItem(Vector2 pos){
    Item item;
    item.pos = pos;
    item.effect = INCREASE_FIRE_RATE;
    item.pickupSound = POWERUP_PICKUP;
    item.radius = 8;
    item.speed = 0;
    item.sprite = POWER_UP_SPRITE;
    compactAddItem(&itemsCArray, &item);
}

void makeOneUpItem(Vector2 pos){
    Item item;
    item.pos = pos;
    item.effect = GAIN_LIFE;
    item.pickupSound = POWERUP_PICKUP;
    item.radius = 8;
    item.speed = 0;
    item.sprite = ONE_UP_SPRITE;
    compactAddItem(&itemsCArray, &item);
}