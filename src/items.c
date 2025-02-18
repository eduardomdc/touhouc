#include "items.h"
#include "touhou.h"
#include <raymath.h>

Item items[MAX_ITEMS] = {0};
CompactArray itemsCArray = {items, sizeof(Item), MAX_ITEMS, 0};

void renderItems(){
    for (int i=0; i < itemsCArray.freeIndex; i++){
        Item item = items[i];
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

void physicsUpdateItems(float dt){
    for (int i=0; i < itemsCArray.freeIndex; i++){
        Item item = items[i];
        item.speed += ITEM_FALL_ACC*dt;
        item = moveItem(item, dt);
        items[i] = item;
    }
}

void updateItems(Item* item){
    for (int i=0; i < itemsCArray.freeIndex; i++){
        Item item = items[i];
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
    }
}

void (*itemEffects[ITEM_EFFECTS_LEN])(Player*) = {
    itemEffectGainPoint,
    itemEffectGainLife,
    itemEffectIncreaseFireRate,
};
void itemEffectGainPoint(Player* player) {
    player->points+= 100;
}
void itemEffectGainLife(Player* player) {
    player->lifes++;
}
void itemEffectIncreaseFireRate(Player* player) {
    playerSetFireRate(player->fireRate*1.2);
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