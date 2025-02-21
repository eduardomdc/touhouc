#include <raylib.h>
#include "networking/client.h"
#include "touhou.h"

int main(int argc, char *argv[]){
    InitWindow(hRes, vRes, "TouhouC");
    SetTargetFPS(fps);
    setupGame(argv[1]);
    while(!WindowShouldClose()){
        renderGame();
        Input input = handleInput();
        updateGame(input);
    }
    if (!isServer){
        closeClient();
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}