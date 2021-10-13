#ifndef MENU_H
#define MENU_H
// standard import (UNIX)
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h> 
#include <string.h>
#include <stdbool.h>
// project import (relative path)
#include "../BetterTerminal/betterTerminal.h"
#include "../mazeGenerator/mazeGenerator.h"
#include "../rawTerminal/rawTerminal.h"
#include "../mazeDisplay/mazeDisplay.h"
#include "../mazeHandler/mazeHandler.h"
// defining all const
#define LEADERBOARD_FILE "./data/game.score" // the relative path to the leaderboad file storing all leaderboard infos
#define MAX_NUMBER_OF_SCORE 10 // the max number of score in the leaderBoard
#define MAZE_CREATION_FOCUS_MAX 5 // the maximal index (menu elements) in the maze creation

/**
 * menuType is a type defined from an enum of all the possibles states of the menu
*/
typedef enum menuTypes {
    SELECTION,
    CREATE_MAZE,
    LOAD_MAZE,
    PLAY,
    EXIT,
    SAVE_MAZE_CHOICE
} menuType;
/**
 * leaderBoardScore is type based on a struct leaderBoardScores used to store the
 * leaderBoard scores (score and player name) after that the player resolved a maze
*/
typedef struct leaderBoardScores {
    char * name;
    int score;
} leaderBoardScore;

/**
 * launch menu will start the menu display in the terminal
*/
void launchMenu();

/**
 * will change the current menu which is displayed
 * 
 * @param newMenu the menu to change the current menu to
*/
void changeMenu(menuType newMenu);

#endif //MENU_H