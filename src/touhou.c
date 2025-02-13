#include "touhou.h"
#include <raymath.h>

void setupGame(){
    gameOver = false;
    player.pos.x = hRes/2;
    player.pos.y = vRes/2;
}

void updateGame(){
    if (!gameOver){
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
        Vector2 inputDirectionNormalized = Vector2Normalize(inputDirection);

        float distance = playerSpeed*GetFrameTime();
        Vector2 playerMovement = Vector2Scale(inputDirectionNormalized, distance);
        player.pos = Vector2Add(player.pos, playerMovement);
    }
}

void renderGame(){
    BeginDrawing();
    ClearBackground(BLACK);
    // painter's algorithm
    renderPlayer();
    EndDrawing();
}

void renderPlayer(){
    DrawCircleV(player.pos, playerSize, RED);
}