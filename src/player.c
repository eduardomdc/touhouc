#include "player.h"
#include "items.h"
#include "timer.h"
#include "touhou.h"
#include "bullets.h"
#include "sprite.h"
#include "assets.h"
#include <stdlib.h>
#include <raymath.h>
#include "networking/packets.h"

Player players[PLAYER_CHARACTER_LEN] = {0};

void setupPlayers(){
    // set up Marisa (host character)
    Player marisa = {
        .character = MARISA,
        .pos = {hRes/2, vRes/2},
        .bulletSpeed = 1500,
        .bulletRadius = 5,
        .fireRate = 15,
        .damage = 10,
        .lives = 3,
        .bulletSpreadAngle = 0.02,
        .fireTimer = createTimer(1.0/15),
        .sprite = SPRITE_MARISA,
        .bulletSprite = BLUE_ARROW_8,
        .hitBoxColor = BLUE,
        .score = 0,
        .alive = true,
        .connected = true
    };
    players[MARISA] = marisa;
    // set up Reimu (client character)
    Player reimu = marisa;
    reimu.character = REIMU;
    reimu.sprite = SPRITE_REIMU;
    reimu.bulletSprite = RED_CARD_12;
    reimu.hitBoxColor = ORANGE;
    if (isServer){
        reimu.connected = false;
    } else {
        reimu.connected = true;
    }
    players[REIMU] = reimu;
}

void movePlayer(Player* player, Vector2 inputDir){
    float distance = playerSpeed*GetFrameTime();
    Vector2 playerMovement = Vector2Scale(inputDir, distance);
    Vector2 newPosition = Vector2Add(player->pos, playerMovement);
    if (onScreen(newPosition, -playerSize)){
        player->pos = newPosition;
    }
}

void updatePlayer(Player* player, Input input){
    updateTimer(&player->fireTimer);
    movePlayer(player, input.dir);
    if (input.firing){
        playerFire(player);
    }
}

void renderPlayers(){
    for (int i=0; i < PLAYER_CHARACTER_LEN; i++){
        Player player = players[i];
        if (!player.connected) continue;
        renderSpriteCentered(&assets.playerSprites[player.sprite], player.pos);
        DrawCircleV(player.pos, playerSize, player.hitBoxColor);
        DrawCircleV(player.pos, playerSize-2, WHITE);
    }
}

void playerFire(Player* player){
    if (player->fireTimer.ready){
        Vector2 dir = {0, -1};
        float random = 2*((float)rand()/(float)(RAND_MAX))-1;
        float exitAngle = random*player->bulletSpreadAngle;
        dir = Vector2Rotate(dir, exitAngle);
        Bullet bullet = {.player = player->character, .pos = player->pos, .radius = player->bulletRadius, .direction = dir, .speed = player->bulletSpeed, .sprite = player->bulletSprite};
        compactAddItem(&compactPlayerBulletArray, &bullet);
        PlaySound(assets.soundEffects[SFX_PLAYER_FIRE]);
        resetTimer(&player->fireTimer);
        sendUDPSfx(SFX_PLAYER_FIRE);
    }
}

void playerSetFireRate(Player* player, float fireRate){
    player->fireRate = fireRate;
    player->fireTimer.duration = 1/fireRate;
}

void playerPickUp(short item, Player* player){
    PlaySound(assets.soundEffects[itemData[item].pickupSound]);
    itemEffects[itemData[item].effect](player);
    sendTcpPlayerItemPickup(player->character, item);
}

void playerGetHit(Player* player){
    if (player->lives > 0){
        player->lives -= 1;
        PlaySound(assets.soundEffects[SFX_PLAYER_HIT]);
        sendTcpPlayerHit(player->character);
    } else if (player->lives == 0 && !gameOver){
        gameOver = true;
        PlaySound(assets.soundEffects[SFX_PLAYER_DEATH]);
    }
}
