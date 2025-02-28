#include "menu.h"
#include "touhou.h"
#include "assets.h"
#include "networking/client.h"
#include "networking/server.h"

void (*runWindow[MENU_WINDOW_LEN])() = {
    windowMain,
    windowJoin
};

void runMenu(Menu* menu){
    UpdateMusicStream(assets.bgm[MENU_THEME]);
    runWindow[menu->window](menu);
}

Menu enterMenu(){
    Menu menu = {0};
    PlayMusicStream(assets.bgm[MENU_THEME]);
    return menu;
}

void closeMenu(Menu* menu){
    StopMusicStream(assets.bgm[MENU_THEME]);
    menu->ended = true;
}
// MAIN WINDOW

void renderMainWindow(Menu menu){
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(assets.backgroundSprites[BACKGROUND_SPRITE_MENU].tex, 0, 0, WHITE);
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
            PlaySound(assets.soundEffects[SOUND_EFFECT_SELECT]);
            break;
        case KEY_DOWN:
            menu->selectedOption++;
            if (menu->selectedOption >= MAIN_MENU_OPTION_LEN){
                menu->selectedOption = 0;
            }
            PlaySound(assets.soundEffects[SOUND_EFFECT_SELECT]);
            break;
        case KEY_SPACE:
            PlaySound(assets.soundEffects[SOUND_EFFECT_OK]);
            switch (menu->selectedOption){
            case MAIN_MENU_OPTION_HOST:
                if (initServer()){
                    closeMenu(menu);
                    isServer = true;
                    setupGame();
                } else {
                    // fail
                }
                
                break;
            case MAIN_MENU_OPTION_JOIN:
                menu->selectedOption = 0;
                menu->window = MENU_WINDOW_JOIN;
                break;
            case MAIN_MENU_OPTION_QUIT:
                closeMenu(menu);
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
    DrawTexture(assets.backgroundSprites[BACKGROUND_SPRITE_MENU].tex, 0, 0, WHITE);
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
                PlaySound(assets.soundEffects[SOUND_EFFECT_SELECT]);
                menu->ipBox.text[menu->ipBox.caret] = (char)key;
                menu->ipBox.text[menu->ipBox.caret+1] = '\0';
                menu->ipBox.caret++;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)){
            menu->ipBox.caret--;
            PlaySound(assets.soundEffects[SOUND_EFFECT_BACKSPACE]);
            if (menu->ipBox.caret < 0) menu->ipBox.caret = 0;
            menu->ipBox.text[menu->ipBox.caret] = '\0';
        }
    } else if (menu->selectedOption == JOIN_MENU_OPTION_JOIN){
        if (IsKeyPressed(KEY_SPACE)){
            if (initClient(menu->ipBox.text)){
                closeMenu(menu);
                isServer = false;
                setupGame();
            } else {
                // failed to connect
            }
        }
    }
    if (IsKeyPressed(KEY_UP)){
        PlaySound(assets.soundEffects[SOUND_EFFECT_SELECT]);
        menu->selectedOption--;
        if (menu->selectedOption < 0){
            menu->selectedOption = JOIN_MENU_OPTION_LEN-1;
        }
    }
    if (IsKeyPressed(KEY_DOWN)){
        PlaySound(assets.soundEffects[SOUND_EFFECT_SELECT]);
        menu->selectedOption++;
        if (menu->selectedOption >= JOIN_MENU_OPTION_LEN){
            menu->selectedOption = 0;
        }
    }
}