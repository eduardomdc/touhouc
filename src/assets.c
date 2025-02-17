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

void loadSoundEffects(){
    assets.soundEffects[PLAYER_FIRE] = LoadSound("../assets/sounds/player_fire.wav");
    assets.soundEffects[PLAYER_HIT] = LoadSound("../assets/sounds/player_hit.wav");
    assets.soundEffects[PLAYER_DEATH] = LoadSound("../assets/sounds/player_death.wav");
    assets.soundEffects[ENEMY_HIT] = LoadSound("../assets/sounds/enemy_hit.wav");
    assets.soundEffects[POINT_PICKUP] = LoadSound("../assets/sounds/point_pickup.wav");
    assets.soundEffects[POWERUP_PICKUP] = LoadSound("../assets/sounds/powerup.wav");
}

void loadBGM(){
    assets.bgm[DESERTED_HELL] = LoadMusicStream("../assets/sounds/bgm_deserted_hell.wav");
}

void loadAssets(){
    //Sprites
    loadBulletSprites();
    loadPlayerSprites();
    loadEnemySprites();
    loadBackgroundSprites();
    //Sounds
    loadSoundEffects();
    loadBGM();
}