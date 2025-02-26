#include "assets.h"

Assets assets = {0};

void loadBulletSprites(){
    assets.bulletSprites[FIRE_BALL_8] = loadSprite("../assets/textures/fire8.png");
    assets.bulletSprites[FIRE_BALL_12] = loadSprite("../assets/textures/fire12.png");
    assets.bulletSprites[BLUE_ARROW_8] = loadSprite("../assets/textures/spear8.png");
    assets.bulletSprites[RED_CARD_12] = loadSprite("../assets/textures/card12.png");
}

void loadPlayerSprites(){
    assets.playerSprites[SPRITE_MARISA] = loadSprite("../assets/textures/marisa23x35.png");
    assets.playerSprites[SPRITE_REIMU] = loadSprite("../assets/textures/reimu28x42.png");
}

void loadEnemySprites(){
    assets.enemySprites[ANGEL] = loadSprite("../assets/textures/enemy22x25.png");
}

void loadInterfaceSprites(){
    assets.interfaceSprites[LIFE_ICON] = loadSprite("../assets/textures/life_icon.png");
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

void loadItemSprites(){
    assets.itemSprites[POINT_SPRITE] = loadSprite("../assets/textures/point12x12.png");
    assets.itemSprites[POWER_UP_SPRITE] = loadSprite("../assets/textures/powerup16x16.png");
    assets.itemSprites[ONE_UP_SPRITE] = loadSprite("../assets/textures/oneup16x16.png");
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
    loadInterfaceSprites();
    loadItemSprites();
    //Sounds
    loadSoundEffects();
    loadBGM();
}
