#include "player.h"
#include "touhou.h"
#include "bullets.h"
#include "sprite.h"
#include <raymath.h>

Player player = {0};

void setupPlayer(){
    player.pos.x = hRes/2;
    player.pos.y = vRes/2;
    player.bulletSpeed = 500;
    player.bulletRadius = 5;
    player.fireRate = 15;
    player.lifes = 3;
    player.fireTimer = createTimer(1/player.fireRate);
    player.sprite = loadSprite("../assets/textures/marisa23x35.png");
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
    renderSpriteCentered(&player.sprite, player.pos);
    DrawCircleV(player.pos, playerSize, playerColor);
    DrawCircleV(player.pos, playerSize-2, WHITE);
}

void playerFire(){
    if (player.fireTimer.ready){
        Bullet bullet;
        bullet.pos = player.pos;
        bullet.active = true;
        bullet.radius = player.bulletRadius;
        Vector2 dir = {0, -1};
        bullet.direction = dir;
        bullet.speed = player.bulletSpeed;
        addBulletToList(bullet, &playerBulletList);
        resetTimer(&player.fireTimer);
    }
}

#include <stdio.h>
void playerGetHit(){
    player.lifes -= 1;
    if (player.lifes < 0){
        gameOver = true;
    }
}