#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

// standard
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
// custom
#include "../maze/maze.h"
#include "../mazeDisplay/mazeDisplay.h"

typedef struct
{
    int width;
    int height;

    int ** columns;
} matrix;

typedef struct {
    int ineighbour1;
    int jneighbour1;
    int ineighbour2;
    int jneighbour2;
} wall;

maze generateMaze(int width, int height);

#endif //MAZE_GENERATOR_H