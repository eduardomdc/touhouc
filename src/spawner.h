#ifndef SPAWNER_H
#define SPAWNER_H

#include "timer.h"
#include "enemy.h"

#define MAX_ENEMIES 10

typedef struct Spawner {
    Timer enemySpawnTimer;
    Timer oneUpSpawnTimer;
    Timer powerUpSpawnTimer;
} Spawner;

void setupSpawner();
void updateSpawner();
void spawnEnemy();

extern Spawner spawner;
extern Enemy enemyList[MAX_ENEMIES];

#endif