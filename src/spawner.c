#include "spawner.h"
#include "touhou.h"
#include "enemy_factory.h"

Spawner spawner;
Enemy enemyList[MAX_ENEMIES] = {0};

void setupSpawner(){
    spawner.enemySpawnTimer = createTimer(2);
}

void addEnemy(Enemy enemy){
    for (int i=0; i < MAX_ENEMIES; i++){
        Enemy thisEnemy = enemyList[i];
        if (!thisEnemy.alive){
            enemyList[i] = enemy;
            return;
        }
    }
    // failed to add enemy
}

#include <stdio.h>

void spawnEnemy(){
    Vector2 spawnLocation = {GetRandomValue(10, hRes-10), GetRandomValue(10, vRes/4)};
    Enemy newEnemy = makeEnemy(spawnLocation);
    addEnemy(newEnemy);
}

void updateSpawner(){
    updateTimer(&spawner.enemySpawnTimer);
    if (spawner.enemySpawnTimer.ready){
        spawnEnemy();
        resetTimer(&spawner.enemySpawnTimer);
    }
}