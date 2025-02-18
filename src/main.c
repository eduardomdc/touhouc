#include <raylib.h>
#include "networking/client.h"
#include "touhou.h"

int main(int argc, char *argv[]){
    InitWindow(hRes, vRes, "TouhouC");
    SetTargetFPS(fps);
    setupGame(argv[1]);
    while(!WindowShouldClose()){
        renderGame();
        updateGame();
    }
    if (!isServer){
        closeClient();
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}