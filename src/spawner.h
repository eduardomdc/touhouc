#ifndef SPAWNER_H
#define SPAWNER_H

#include "timer.h"
#include "enemy.h"
#include "compact_array.h"

#define MAX_ENEMIES 6

typedef struct Spawner {
    Timer angelSpawnTimer;
    Timer jiangshiSpawnTimer;
    Timer oneUpSpawnTimer;
    Timer powerUpSpawnTimer;
} Spawner;

void setupSpawner();
void updateSpawner();
void spawnEnemy();

extern Spawner spawner;
extern Enemy enemyList[MAX_ENEMIES];
extern CompactArray compactEnemyArray;

#endif
