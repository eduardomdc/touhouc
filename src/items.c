#include "items.h"
#include "touhou.h"
#include <raymath.h>

Item items[MAX_ITEMS] = {0};

ItemData itemTypes[ITEM_TYPE_LEN] = {0};
itemTypes[SCORE_ITEM] = {GAIN_POINT, POINT_PICKUP, POINT_SPRITE, 6};
itemTypes[ONE_UP_ITEM] = {GAIN_LIFE, POWERUP_PICKUP, ONE_UP_SPRITE, 8};
itemTypes[POWER_UP_ITEM] = {INCREASE_FIRE_RATE, POWERUP_PICKUP, POWER_UP_SPRITE, 8};

CompactArray itemsCArray = {items, sizeof(Item), MAX_ITEMS, 0};

void renderItems(){
    for (int i=0; i < itemsCArray.freeIndex; i++){
        Item item = items[i];
        renderSpriteCentered(&assets.itemSprites[itemTypes[item.type].sprite], item.pos);
    }
}

bool collidingWithPlayer(Item item){
    float distance = Vector2Distance(item.pos, player.pos);
    if (distance < (itemData[item.type].radius + playerSize)){
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
        ItemData data = itemData[item.type];
        if (collidingWithPlayer(item) && player.alive){
            PlaySound(assets.soundEffects[data.pickupSound]);
            itemEffects[data.effect](&player);
            compactRemoveItem(&itemsCArray, i);
            continue;
        }
        if (!onScreen(item.pos, data.radius)){
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
    item.type = SCORE_ITEM;
    item.pos = pos;
    item.speed = 0;
    compactAddItem(&itemsCArray, &item);
}

void makePowerUpItem(Vector2 pos){
    Item item;
    item.type = POWER_UP_ITEM;
    item.pos = pos;
    item.speed = 0;
    compactAddItem(&itemsCArray, &item);
}

void makeOneUpItem(Vector2 pos){
    Item item;
    item.type = ONE_UP_ITEM;
    item.pos = pos;
    item.speed = 0;
    compactAddItem(&itemsCArray, &item);
}