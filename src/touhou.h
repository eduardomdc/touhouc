#include "raylib.h"

#define MAX_ENEMY_PROJECTILES 100
#define MAX_PLAYER_PROJECTILES 100

static const int vRes = 700;
static const int hRes = 500;
static const int fps = 60;
static const int playerSize = 10;
static const float playerSpeed = 100;

void setupGame();
void renderGame();
void renderPlayer();
void updateGame();

typedef struct Player {
    Vector2 pos;
    unsigned int lifes;
} Player;

typedef struct Projectile {
    Vector2 pos;
    Vector2 vel;
    float radius;
    bool active;
} Projectile;

static bool gameOver;
static Player player;
static Projectile enemyProjectiles[MAX_ENEMY_PROJECTILES] = {0};
static Projectile playerProjectiles[MAX_PLAYER_PROJECTILES] = {0};