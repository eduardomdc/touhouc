#include <raylib.h>
#include "touhou.h"
#include "player.h"
#include "bullets.h"
#include "spawner.h"
#include "enemy.h"
#include "assets.h"
#include "items.h"
#include "networking/server.h"
#include <time.h>
#include <stdio.h>
#include <string.h>

bool gameOver;
bool isServer = false;

void setupGame(char* hostType){
    if (strcmp(hostType, "server") == 0){
        isServer = true;
        SetRandomSeed(time(NULL));
        initServer(); // server side
        InitAudioDevice();
        gameOver = false;
        loadAssets();
        setupPlayer();
        setupSpawner();
        PlayMusicStream(assets.bgm[DESERTED_HELL]);
    }
    else if (strcmp(hostType, "client") == 0){
        return;
    }
}

void updateGame(){
    if (gameServer.active && !gameServer.clientIsConnected){
        serverCheckForClientConnection();
    }
    UpdateMusicStream(assets.bgm[DESERTED_HELL]);
    // physics
    float delta = GetFrameTime();
    physicsUpdateItems(delta);
    physicsUpdateBullets(&compactPlayerBulletArray, delta);
    physicsUpdateBullets(&compactEnemyBulletArray, delta);
    // update collision detection and timers
    if (isServer){
        if (!gameOver){
            updatePlayer();
        }
        updateEnemies();
        updateItems();
        updatePlayerBullets();
        updateEnemyBullets();
        updateSpawner();
    }
    
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

void renderUI(){
    Sprite* lifeIcon = &assets.interfaceSprites[LIFE_ICON];
    Vector2 lifeIconPos = {0,vRes-lifeIcon->tex.height};
    for (int i=0; i < player.lifes; i++){
        renderSprite(lifeIcon, lifeIconPos);
        lifeIconPos.x += lifeIcon->tex.width;
    }
    char score_text[28];
    sprintf(score_text, "Score: %d", player.points);
    DrawText(score_text, 0, 0, 18, WHITE);
}

void renderGame(){
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(assets.backgroundSprites[CHAPEL].tex, 0, 0, WHITE);
    renderBulletCArray(compactPlayerBulletArray);
    renderItems();
    renderPlayer();
    renderBulletCArray(compactEnemyBulletArray);
    renderEnemies();
    renderUI();
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