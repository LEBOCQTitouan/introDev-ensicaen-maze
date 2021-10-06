#ifndef MENU_H
#define MENU_H

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h> 
#include <string.h>
#include <stdbool.h>

#include "../mazeHandler/mazeHandler.h"
#include "../rawTerminal/rawTerminal.h"
#include "../BetterTerminal/betterTerminal.h"
#include "../mazeDisplay/mazeDisplay.h"
#include "../mazeGenerator/mazeGenerator.h"

typedef enum {
    SELECTION,
    CREATE_MAZE,
    LOAD_MAZE,
    PLAY,
    EXIT
} menuType;

void launchMenu();

void changeMenu(menuType newMenu);

#endif //MENU_H