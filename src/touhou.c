#include "touhou.h"
#include "player.h"
#include <raymath.h>

void setupGame(){
    gameOver = false;
    setupPlayer();
}

void updateGame(){
    if (!gameOver){
        updatePlayer();
    }
}

void renderGame(){
    BeginDrawing();
    ClearBackground(BLACK);
    // painter's algorithm
    renderPlayer();
    EndDrawing();
}