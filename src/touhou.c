#include <raylib.h>
#include "touhou.h"
#include "player.h"
#include "bullets.h"

void setupGame(){
    gameOver = false;
    setupPlayer();
}

void updateGame(){
    if (!gameOver){
        updatePlayer();
    }
    updateBulletList(&enemyBulletList);
    updateBulletList(&playerBulletList);
}

void renderGame(){
    BeginDrawing();
    ClearBackground(BLACK);
    // painter's algorithm
    renderBulletList(enemyBulletList);
    renderBulletList(playerBulletList);
    renderPlayer();
    EndDrawing();
}