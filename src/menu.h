#ifndef MENU_H
#define MENU_H

#define MAX_SIZE_IP 39 // ipv6 has 39 characters

#include <raylib.h>

typedef enum {
    MENU_WINDOW_MAIN,
    MENU_WINDOW_JOIN,
    MENU_WINDOW_LEN
} MenuWindow;

typedef enum {
    MENU_OPTION_HOST,
    MENU_OPTION_JOIN,
    MENU_OPTION_QUIT,
    MENU_OPTION_LEN
} MainMenuOption;

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

void runMenu(Menu* menu);
extern void (*runWindow[MENU_WINDOW_LEN])(Menu* menu);

void windowMain(Menu* menu);
void windowJoin(Menu* menu);

#endif