#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

/* standard */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
/* custom */
#include "../maze/maze.h"
#include "../mazeDisplay/mazeDisplay.h"
#include "../mazeHandler/mazeHandler.h"

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

/**
 * difficulty is an enum used to define the maze difficulty
*/
typedef enum generation_difficulties {
    NORMAL_MODE,
    HARDCORE_MODE
} generation_difficulty;

maze generateMaze(int width, int height, char * name, generation_difficulty difficulty);

#endif /* MAZE_GENERATOR_H */