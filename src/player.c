#include "player.h"
#include "touhou.h"
#include "bullets.h"
#include <raymath.h>
#include <stdio.h>

Player player = {0};

void setupPlayer(){
    player.pos.x = hRes/2;
    player.pos.y = vRes/2;
    player.bulletSpeed = 500;
    player.fireRate = 15;
    player.lifes = 3;
    player.fireTimer = createTimer(1/player.fireRate);
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
    player.pos = Vector2Add(player.pos, playerMovement);
}

void updatePlayer(){
    updateTimer(&player.fireTimer);
    handleInput();
}

void renderPlayer(){
    DrawCircleV(player.pos, playerSize, playerColor);
}

void playerFire(){
    if (player.fireTimer.ready){
        Bullet bullet;
        bullet.pos = player.pos;
        bullet.active = true;
        bullet.radius = 2;
        Vector2 dir = {0, -1};
        bullet.direction = dir;
        bullet.speed = player.bulletSpeed;
        addBulletToList(bullet, &playerBulletList);
        resetTimer(&player.fireTimer);
    }
}