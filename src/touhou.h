#include <raylib.h>

#define MAX_ENEMY_PROJECTILES 100
#define MAX_PLAYER_PROJECTILES 100

static const int vRes = 700;
static const int hRes = 500;
static const int fps = 60;

void setupGame();
void renderGame();
void updateGame();


typedef struct Projectile {
    Vector2 pos;
    Vector2 vel;
    float radius;
    bool active;
} Projectile;

static bool gameOver;
static Projectile enemyProjectiles[MAX_ENEMY_PROJECTILES] = {0};
static Projectile playerProjectiles[MAX_PLAYER_PROJECTILES] = {0};