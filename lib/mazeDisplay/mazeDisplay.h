#ifndef DISPLAY_H
#define DISPLAY_H

// standard import
// custom import
#include "../BetterTerminal/betterTerminal.h"
#include "../maze/maze.h"

typedef enum {
    MINIMALIST_MAZE_DISPLAY,
    DEFAULT_MAZE_DISPLAY,
    DETAILLED_MAZE_DISPLAY
} mazeDisplayType;

void displayMaze(maze maze, mazeDisplayType displayType);

#endif // DISPLAY_H