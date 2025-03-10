#include "spawner.h"
#include "compact_array.h"
#include "timer.h"
#include "touhou.h"
#include "enemy.h"
#include "items.h"

Spawner spawner;
Enemy enemyList[MAX_ENEMIES] = {0};
CompactArray compactEnemyArray = {enemyList, sizeof(Enemy), MAX_ENEMIES, 0};

void setupSpawner(){
    spawner.angelSpawnTimer = createTimer(4);
    spawner.jiangshiSpawnTimer = createTimer(10);
    spawner.oneUpSpawnTimer = createTimer(10);
    spawner.powerUpSpawnTimer = createTimer(6);
}

void spawnEnemy(Enemy enemyData, Timer* spawnTimer){
    updateTimer(spawnTimer);
    if (spawnTimer->ready){
        Vector2 spawnLocation = {GetRandomValue(10, hRes-10), -10};
        Enemy newEnemy = enemyData;
        newEnemy.pos = spawnLocation;
        compactAddItem(&compactEnemyArray, &newEnemy);
        resetTimer(spawnTimer);
    }
}

void spawnItem(int itemType, Timer* spawnTimer){
    updateTimer(spawnTimer);
    if (spawnTimer->ready){
        Vector2 spawnLocation = {GetRandomValue(10, hRes-10), 0};
        makeItem(itemType, spawnLocation);
        resetTimer(spawnTimer);
    }
}

void updateSpawner(){
    spawnItem(ONE_UP_ITEM, &spawner.oneUpSpawnTimer);
    spawnItem(POWER_UP_ITEM, &spawner.powerUpSpawnTimer);
    spawnEnemy(enemyData.angel, &spawner.angelSpawnTimer);
    spawnEnemy(enemyData.jiangshi, &spawner.jiangshiSpawnTimer);
}
