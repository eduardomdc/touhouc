#include <raylib.h>
#include "touhou.h"
#include "player.h"
#include "bullets.h"
#include "spawner.h"
#include "enemy.h"
#include <time.h>

bool gameOver;

void setupGame(){
    SetRandomSeed(time(NULL));
    gameOver = false;
    setupPlayer();
    setupSpawner();
}

void updateGame(){
    if (!gameOver){
        updatePlayer();
    }
    updateBulletList(&enemyBulletList);
    updateBulletList(&playerBulletList);
    updateSpawner();
}

#include <stdio.h>
void renderEnemies(){
    for (int i=0; i < MAX_ENEMIES; i++){
        if (enemyList[i].alive){
            renderEnemy(enemyList[i]);
        }
    }
}

void renderGame(){
    BeginDrawing();
    ClearBackground(BLACK);
    // painter's algorithm
    renderBulletList(enemyBulletList);
    renderBulletList(playerBulletList);
    renderEnemies();
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