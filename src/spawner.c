#include "spawner.h"
#include "compact_array.h"
#include "touhou.h"
#include "enemy_factory.h"
#include "items.h"

Spawner spawner;
Enemy enemyList[MAX_ENEMIES] = {0};
CompactArray compactEnemyArray = {enemyList, sizeof(Enemy), MAX_ENEMIES, 0};

void setupSpawner(){
    spawner.enemySpawnTimer = createTimer(4);
    spawner.oneUpSpawnTimer = createTimer(10);
    spawner.powerUpSpawnTimer = createTimer(6);
}

void spawnEnemy(){
    Vector2 spawnLocation = {GetRandomValue(10, hRes-10), -10};
    Enemy newEnemy = makeEnemy(spawnLocation);
    compactAddItem(&compactEnemyArray, &newEnemy);
}

void updateSpawner(){
    updateTimer(&spawner.enemySpawnTimer);
    updateTimer(&spawner.oneUpSpawnTimer);
    updateTimer(&spawner.powerUpSpawnTimer);
    if (spawner.enemySpawnTimer.ready){
        spawnEnemy();
        resetTimer(&spawner.enemySpawnTimer);
    }
    if (spawner.oneUpSpawnTimer.ready){
        Vector2 spawnLocation = {GetRandomValue(10, hRes-10), 0};
        makeOneUpItem(spawnLocation);
        resetTimer(&spawner.oneUpSpawnTimer);
    }
    if (spawner.powerUpSpawnTimer.ready){
        Vector2 spawnLocation = {GetRandomValue(10, hRes-10), 0};
        makePowerUpItem(spawnLocation);
        resetTimer(&spawner.powerUpSpawnTimer);
    }
}
