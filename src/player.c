#include "player.h"
#include "items.h"
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
    Player marisa;
    marisa.character = MARISA;
    marisa.pos.x = hRes/2;
    marisa.pos.y = vRes/2;
    marisa.bulletSpeed = 1000;
    marisa.bulletRadius = 5;
    marisa.fireRate = 15;
    marisa.lifes = 3;
    marisa.bulletSpreadAngle = 0.05;
    marisa.fireTimer = createTimer(1/marisa.fireRate);
    marisa.sprite = SPRITE_MARISA;
    marisa.points = 0;
    marisa.alive = true;
    marisa.connected = true;
    players[MARISA] = marisa;
    // set up Reimu (client character)
    Player reimu;
    reimu.character = REIMU;
    reimu.pos.x = hRes/2;
    reimu.pos.y = vRes/2;
    reimu.bulletSpeed = 1000;
    reimu.bulletRadius = 5;
    reimu.fireRate = 15;
    reimu.lifes = 3;
    reimu.bulletSpreadAngle = 0.05;
    reimu.fireTimer = createTimer(1/reimu.fireRate);
    reimu.sprite = SPRITE_REIMU;
    reimu.points = 0;
    reimu.alive = true;
    reimu.connected = false;
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
        DrawCircleV(player.pos, playerSize, playerColor);
        DrawCircleV(player.pos, playerSize-2, WHITE);
    }
}

void playerFire(Player* player){
    if (player->fireTimer.ready){
        Bullet bullet;
        bullet.pos = player->pos;
        bullet.radius = player->bulletRadius;
        Vector2 dir = {0, -1};
        float random = 2*((float)rand()/(float)(RAND_MAX))-1;
        float exitAngle = random*player->bulletSpreadAngle;
        dir = Vector2Rotate(dir, exitAngle);
        bullet.direction = dir;
        bullet.speed = player->bulletSpeed;
        bullet.sprite = BLUE_ARROW_8;
        compactAddItem(&compactPlayerBulletArray, &bullet);
        PlaySound(assets.soundEffects[PLAYER_FIRE]);
        resetTimer(&player->fireTimer);
        sendUDPPlayerFire();
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
    if (player->lifes > 0){
        player->lifes -= 1;
        PlaySound(assets.soundEffects[PLAYER_HIT]);
        sendTcpPlayerHit(player->character);
    } else if (player->lifes == 0 && !gameOver){
        //gameOver = true;
        //PlaySound(assets.soundEffects[PLAYER_DEATH]);
    }
}
