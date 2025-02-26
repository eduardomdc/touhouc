#include "spawner.h"
#include "compact_array.h"
#include "touhou.h"
#include "enemy_factory.h"
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

void spawnAngel(){
    Vector2 spawnLocation = {GetRandomValue(10, hRes-10), -10};
    Enemy newEnemy = makeAngel(spawnLocation);
    compactAddItem(&compactEnemyArray, &newEnemy);
}

void spawnJiangshi(){
    Vector2 spawnLocation = {GetRandomValue(10, hRes-10), -10};
    Enemy newEnemy = makeJiangshi(spawnLocation);
    compactAddItem(&compactEnemyArray, &newEnemy);
}

void updateSpawner(){
    updateTimer(&spawner.angelSpawnTimer);
    updateTimer(&spawner.jiangshiSpawnTimer);
    updateTimer(&spawner.oneUpSpawnTimer);
    updateTimer(&spawner.powerUpSpawnTimer);
    if (spawner.angelSpawnTimer.ready){
        spawnAngel();
        resetTimer(&spawner.angelSpawnTimer);
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
