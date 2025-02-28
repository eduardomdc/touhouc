#include <raylib.h>
#include "networking/client.h"
#include "touhou.h"
#include "menu.h"
#include "assets.h"

void exitGame(){
    if (!isServer){
        closeClient();
    }
    CloseAudioDevice();
    CloseWindow();
}

int main(){
    InitWindow(hRes, vRes, "TouhouC");
    SetTargetFPS(fps);
    InitAudioDevice();
    loadAssets();
    Menu menu = enterMenu();
    while(!WindowShouldClose() && !gameClosed){
        if (!menu.ended){
            runMenu(&menu);
        } else {
            renderGame();
            Input input = handleInput();
            updateGame(input);
        }
    }
    exitGame();
    return 0;
}