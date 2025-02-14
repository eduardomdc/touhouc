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
    loadSprites();
    setupPlayer();
    setupSpawner();
}

Sprite enemySprite = {0};
Sprite enemyBulletSprite = {0};
Sprite playerBulletSprite = {0};
Texture2D backgroundTexture = {0};
void loadSprites(){
    enemySprite = loadSprite("../assets/textures/enemy22x25.png");
    backgroundTexture = LoadTexture("../assets/textures/background500x700.png");
    playerBulletSprite = loadSprite("../assets/textures/spear8.png");
    enemyBulletSprite = loadSprite("../assets/textures/fire8.png");
}

void updateGame(){
    if (!gameOver){
        updatePlayer();
    }
    updateEnemies();
    updatePlayerBulletList();
    updateEnemyBulletList();
    updateSpawner();
}

void updateEnemies(){
    for(int i=0; i < MAX_ENEMIES; i++){
        Enemy* enemy = &enemyList[i];
        if (enemy->alive){
            updateEnemy(enemy);
        }
    }
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
    DrawTexture(backgroundTexture, 0, 0, WHITE);
    // painter's algorithm
    
    renderBulletList(playerBulletList);
    renderPlayer();
    renderBulletList(enemyBulletList);
    renderEnemies();
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