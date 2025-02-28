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
                menu->selectedOption = MAIN_MENU_OPTION_LEN-1;
            }
            break;
        case KEY_DOWN:
            menu->selectedOption++;
            if (menu->selectedOption >= MAIN_MENU_OPTION_LEN){
                menu->selectedOption = 0;
            }
            break;
        case KEY_SPACE:
            switch (menu->selectedOption){
            case MAIN_MENU_OPTION_HOST:
                menu->ended = true;
                setupGame(true, "");
                break;
            case MAIN_MENU_OPTION_JOIN:
                menu->selectedOption = 0;
                menu->window = MENU_WINDOW_JOIN;
                break;
            case MAIN_MENU_OPTION_QUIT:
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

void renderJoinWindow(Menu menu){
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Server Address", hRes/6, vRes/3, 16, WHITE);
    DrawRectangle(hRes/6, vRes/3+32, 20*16, 16, WHITE);
    DrawText(menu.ipBox.text, hRes/6, vRes/3+32, 16, BLACK);
    if (menu.selectedOption == JOIN_MENU_OPTION_IP){
        int caretPosition = MeasureText(menu.ipBox.text, 16);
        DrawRectangle(hRes/6+caretPosition+2, vRes/3+34, 8, 12, BLACK);
        DrawText(">", hRes/6-16, vRes/3+32, 16, WHITE);
    } else {
        DrawText(">", hRes/6-16, vRes/3+64, 16, WHITE);
    }
    DrawText("Join Server", hRes/6, vRes/3+64, 16, WHITE);
    EndDrawing();
}

#include <stdio.h>

void windowJoin(Menu* menu){
    renderJoinWindow(*menu);
    if (menu->selectedOption == JOIN_MENU_OPTION_IP){
        int key = GetCharPressed();
        while (key) {
            if ((key > 32) && (key <= 125) && (menu->ipBox.caret < MAX_SIZE_IP))
            {
                menu->ipBox.text[menu->ipBox.caret] = (char)key;
                menu->ipBox.text[menu->ipBox.caret+1] = '\0';
                menu->ipBox.caret++;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)){
            menu->ipBox.caret--;
            if (menu->ipBox.caret < 0) menu->ipBox.caret = 0;
            menu->ipBox.text[menu->ipBox.caret] = '\0';
        }
    } else if (menu->selectedOption == JOIN_MENU_OPTION_JOIN){
        if (IsKeyPressed(KEY_SPACE)){
            setupGame(false, menu->ipBox.text);
            menu->ended = true;
        }
    }
    if (IsKeyPressed(KEY_UP)){
        menu->selectedOption--;
        if (menu->selectedOption < 0){
            menu->selectedOption = JOIN_MENU_OPTION_LEN-1;
        }
    }
    if (IsKeyPressed(KEY_DOWN)){
        menu->selectedOption++;
        if (menu->selectedOption >= JOIN_MENU_OPTION_LEN){
            menu->selectedOption = 0;
        }
    }
}