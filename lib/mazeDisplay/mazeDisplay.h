#ifndef DISPLAY_H
#define DISPLAY_H

// standard import
// custom import
#include "../BetterTerminal/betterTerminal.h"
#include "../maze/maze.h"
#include "../mazeHandler/mazeHandler.h"

/**
 * the displayMaze() function is a function used to display the maze (cf maze.h) in the terminal
 * 
 * @param maze is the maze to display
*/
void displayMaze(maze maze);

/**
 * the displayMazeWithPlayer() function is a function used while a game (using the maze) is started (it display the player and other in game elements)
*/
void displayMazeWithPlayer(mazeHandler handler);

#endif // DISPLAY_H