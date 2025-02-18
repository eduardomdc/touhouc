#include <raylib.h>
#include "touhou.h"

int main(int argc, char *argv[]){
    InitWindow(hRes, vRes, "TouhouC");
    SetTargetFPS(fps);
    setupGame(argv[1]);
    while(!WindowShouldClose()){
        renderGame();
        updateGame();
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}