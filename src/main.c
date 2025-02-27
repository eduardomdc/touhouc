#include <raylib.h>
#include "networking/client.h"
#include "touhou.h"
#include "menu.h"

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
    Menu menu = {0};
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