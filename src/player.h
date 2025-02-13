#include <raylib.h>

static const int playerSize = 5;
static const float playerSpeed = 100;

void setupPlayer();
void renderPlayer();
void updatePlayer();

typedef struct Player {
    Vector2 pos;
    unsigned int lifes;
} Player;

static Player player;