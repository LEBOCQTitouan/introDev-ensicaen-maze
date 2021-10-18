#ifndef MAZE_HANDLER_H
#define MAZE_HANDLER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../maze/maze.h"

#define SAVE_FOLDER "./data/maze/"
#define MAZE_EXT ".cfg"

typedef struct {
    int x;
    int y;
} coord;

typedef struct
{
    coord position;
} player;

typedef struct {
    player mazePlayer;
    maze * maze;
    int score;
} mazeHandler;

typedef enum {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
} directionPlayer;

typedef enum {
    DEFAULT,
    HARDCORE
} difficulty;

typedef struct {
    coord * path;
    int size;
} path;

maze loadMAze(char * name);

int saveMaze(maze m);

bool compareMaze(maze m1, maze m2);

mazeHandler initMazeMovement(maze * m);

bool movePlayer(mazeHandler * handler, directionPlayer direction);

bool isPossibleDirection(mazeHandler handler, directionPlayer direction);

bool isPossibleDirectionFromCoords(maze * m, int x, int y, directionPlayer direction);

/**
 * the isEntityCoord() function will return the index of the entity at x y or -1 if no entity
*/
int isEntityCoord(maze * m, int x, int y);

#endif //MAZE_HANDLER_H