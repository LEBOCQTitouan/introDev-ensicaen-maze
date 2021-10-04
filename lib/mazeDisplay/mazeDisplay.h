#ifndef DISPLAY_H
#define DISPLAY_H

// standard import
// custom import
#include "../BetterTerminal/betterTerminal.h"
#include "../maze/maze.h"
#include "../mazeHandler/mazeHandler.h"

void displayMaze(maze maze);

void displayMazeWithPlayer(mazeHandler handler);

#endif // DISPLAY_H