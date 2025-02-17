#include "player.h"
#include "touhou.h"
#include "bullets.h"
#include "sprite.h"
#include "assets.h"
#include <stdlib.h>
#include <raymath.h>

Player player = {0};

void setupPlayer(){
    player.pos.x = hRes/2;
    player.pos.y = vRes/2;
    player.bulletSpeed = 1000;
    player.bulletRadius = 5;
    player.fireRate = 15;
    player.lifes = 3;
    player.bulletSpreadAngle = 0.05;
    player.fireTimer = createTimer(1/player.fireRate);
    player.sprite = MARISA;
}

void handleInput(){
    Vector2 inputDirection = {0,0};
    if (IsKeyDown(KEY_UP)){
        inputDirection.y -= 1;
    }
    if (IsKeyDown(KEY_DOWN)){
        inputDirection.y += 1;
    }
    if (IsKeyDown(KEY_LEFT)){
        inputDirection.x -= 1;
    }
    if (IsKeyDown(KEY_RIGHT)){
        inputDirection.x += 1;
    }
    if (IsKeyDown(KEY_A)){
        inputDirection.x += 1;
    }
    if (IsKeyDown(KEY_SPACE)){
        playerFire();
    }

    Vector2 inputDir = Vector2Normalize(inputDirection);

    movePlayer(inputDir);
}

void movePlayer(Vector2 inputDir){
    float distance = playerSpeed*GetFrameTime();
    Vector2 playerMovement = Vector2Scale(inputDir, distance);
    Vector2 newPosition = Vector2Add(player.pos, playerMovement);
    if (onScreen(newPosition, -playerSize)){
        player.pos = newPosition;
    }
}

void updatePlayer(){
    updateTimer(&player.fireTimer);
    handleInput();
}

void renderPlayer(){
    renderSpriteCentered(&assets.playerSprites[player.sprite], player.pos);
    DrawCircleV(player.pos, playerSize, playerColor);
    DrawCircleV(player.pos, playerSize-2, WHITE);
}

void playerFire(){
    if (player.fireTimer.ready){
        Bullet bullet;
        bullet.pos = player.pos;
        bullet.radius = player.bulletRadius;
        Vector2 dir = {0, -1};
        float random = 2*((float)rand()/(float)(RAND_MAX))-1;
        float exitAngle = random*player.bulletSpreadAngle;
        dir = Vector2Rotate(dir, exitAngle);
        bullet.direction = dir;
        bullet.speed = player.bulletSpeed;
        bullet.sprite = BLUE_ARROW_8;
        compactAddItem(&compactPlayerBulletArray, &bullet);
        PlaySound(assets.soundEffects[PLAYER_FIRE]);
        resetTimer(&player.fireTimer);
    }
}

void playerSetFireRate(float fireRate){
    player.fireRate = fireRate;
    player.fireTimer.duration = 1/fireRate;
}

#include <stdio.h>
void playerGetHit(){
    if (player.lifes > 0){
        player.lifes -= 1;
        PlaySound(assets.soundEffects[PLAYER_HIT]);
    } else if (player.lifes == 0 && !gameOver){
        gameOver = true;
        PlaySound(assets.soundEffects[PLAYER_DEATH]);
    }
}