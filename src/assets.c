#include "assets.h"

Assets assets = {0};

void loadBulletSprites(){
    assets.bulletSprites[FIRE_BALL_8] = loadSprite("../assets/textures/fire8.png");
    assets.bulletSprites[FIRE_BALL_12] = loadSprite("../assets/textures/fire12.png");
    assets.bulletSprites[BLUE_ARROW_8] = loadSprite("../assets/textures/spear8.png");
}

void loadPlayerSprites(){
    assets.playerSprites[MARISA] = loadSprite("../assets/textures/marisa23x35.png");
}

void loadEnemySprites(){
    assets.enemySprites[ANGEL] = loadSprite("../assets/textures/enemy22x25.png");
}

void loadBackgroundSprites(){
    assets.backgroundSprites[CHAPEL] = loadSprite("../assets/textures/background500x700.png");
}

void loadAssets(){
    loadBulletSprites();
    loadPlayerSprites();
    loadEnemySprites();
    loadBackgroundSprites();
}