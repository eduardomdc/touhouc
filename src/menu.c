#include "menu.h"
#include "touhou.h"

void (*runWindow[MENU_WINDOW_LEN])() = {
    windowMain,
    windowJoin
};

void runMenu(Menu* menu){
    runWindow[menu->window](menu);
}

// MAIN WINDOW

void renderMainWindow(Menu menu){
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("TouhouCoop", hRes/3-32, vRes/3, 32, WHITE);
    DrawText("Host Game", hRes/3, vRes/2, 16, WHITE);
    DrawText("Join Game", hRes/3, vRes/2+16, 16, WHITE);
    DrawText("Quit Game", hRes/3, vRes/2+32, 16, WHITE);
    DrawText(">", hRes/3-16, vRes/2+16*(menu.selectedOption), 16, WHITE);
    EndDrawing();
}

void windowMain(Menu* menu){
    renderMainWindow(*menu);
    int key;
    while (key = GetKeyPressed()){
        switch (key){
        case KEY_UP:
            menu->selectedOption--;
            if (menu->selectedOption < 0){
                menu->selectedOption = MENU_OPTION_LEN-1;
            }
            break;
        case KEY_DOWN:
            menu->selectedOption++;
            if (menu->selectedOption >= MENU_OPTION_LEN){
                menu->selectedOption = 0;
            }
            break;
        case KEY_SPACE:
            switch (menu->selectedOption){
            case MENU_OPTION_HOST:
                break;
            case MENU_OPTION_JOIN:
                break;
            case MENU_OPTION_QUIT:
                menu->ended = true;
                gameClosed = true;
                break;
            default:
                break;
            }
        default:
            break;
        }
    }   
}

void windowJoin(Menu* menu){
    return;
}