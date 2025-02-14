#include "player.h"
#include "touhou.h"
#include "bullets.h"
#include <raymath.h>
#include <stdio.h>

Player player = {0};

void setupPlayer(){
    player.pos.x = hRes/2;
    player.pos.y = vRes/2;
    player.bulletSpeed = 200;
    player.lifes = 3;
}

void updatePlayer(){
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
    if (IsKeyPressed(KEY_SPACE)){
        printf("fire!\n");
        Bullet bullet;
        bullet.pos = player.pos;
        bullet.active = true;
        bullet.radius = 2;
        Vector2 dir = {0, -1};
        bullet.direction = dir;
        bullet.speed = player.bulletSpeed;
        addBulletToList(bullet, &playerBulletList);
    }

    Vector2 inputDirectionNormalized = Vector2Normalize(inputDirection);

    float distance = playerSpeed*GetFrameTime();
    Vector2 playerMovement = Vector2Scale(inputDirectionNormalized, distance);
    player.pos = Vector2Add(player.pos, playerMovement);
}

void renderPlayer(){
    DrawCircleV(player.pos, playerSize, playerColor);
}