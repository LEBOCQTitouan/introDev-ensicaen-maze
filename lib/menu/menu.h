#ifndef MENU_H
#define MENU_H

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h> 
#include <string.h>
#include <stdbool.h>

#include "../BetterTerminal/betterTerminal.h"
#include "../mazeGenerator/mazeGenerator.h"
#include "../rawTerminal/rawTerminal.h"
#include "../mazeDisplay/mazeDisplay.h"
#include "../mazeHandler/mazeHandler.h"

typedef enum {
    SELECTION,
    CREATE_MAZE,
    LOAD_MAZE,
    PLAY,
    EXIT,
    SAVE_MAZE_CHOICE,
    SAVE_MAZE_LOADING,
    SAVE_MAZE
} menuType;

void launchMenu();

void changeMenu(menuType newMenu);

#endif //MENU_H