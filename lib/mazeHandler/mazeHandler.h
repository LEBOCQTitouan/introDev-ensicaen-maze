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

int saveMaze(char * name, maze m);

bool compareMaze(maze m1, maze m2);

mazeHandler initMazeMovement(maze * m);

bool movePlayer(mazeHandler * handler, directionPlayer direction);

bool isPossibleDirection(mazeHandler handler, directionPlayer direction);

#endif //MAZE_HANDLER_H