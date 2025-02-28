#ifndef MENU_H
#define MENU_H

#include <raylib.h>
#include "touhou.h"

typedef enum {
    MENU_WINDOW_MAIN,
    MENU_WINDOW_JOIN,
    MENU_WINDOW_FAILED_TO_HOST,
    MENU_WINDOW_FAILED_TO_JOIN,
    MENU_WINDOW_LEN
} MenuWindow;

typedef enum {
    MAIN_MENU_OPTION_HOST,
    MAIN_MENU_OPTION_JOIN,
    MAIN_MENU_OPTION_QUIT,
    MAIN_MENU_OPTION_LEN
} MainMenuOption;

typedef enum {
    JOIN_MENU_OPTION_IP,
    JOIN_MENU_OPTION_JOIN,
    JOIN_MENU_OPTION_BACK,
    JOIN_MENU_OPTION_LEN
} JoinMenuOption;

typedef struct IpBox {
    char text[MAX_SIZE_IP+1];
    int caret;
} IpBox;

typedef struct Menu {
    MenuWindow window;
    char selectedOption;
    IpBox ipBox;
    bool ended;
} Menu;

Menu enterMenu();
void closeMenu(Menu* menu);
void runMenu(Menu* menu);
extern void (*runWindow[MENU_WINDOW_LEN])(Menu* menu);

void windowMain(Menu* menu);
void windowJoin(Menu* menu);
void windowFailedToHost(Menu* menu);
void windowFailedToJoin(Menu* menu);

#endif