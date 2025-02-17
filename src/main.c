#include <raylib.h>
#include "touhou.h"

int main(){
    InitWindow(hRes, vRes, "TouhouC");
    SetTargetFPS(fps);
    setupGame();
    while(!WindowShouldClose()){
        renderGame();
        updateGame();
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}