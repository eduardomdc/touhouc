#include <raylib.h>
#include "touhou.h"
#include "player.h"
#include "bullets.h"

bool gameOver;

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

bool onScreen(Vector2 pos, float radius){
    // checks if a circle is visible in the game screen
    if (pos.x+radius < 0){
        return false;
    }
    if (pos.x-radius > hRes){
        return false;
    }
    if (pos.y+radius < 0){
        return false;
    }
    if (pos.y-radius > vRes){
        return false;
    }
    return true;
}