#include "lib/minunit/minunit.h"
// standard import
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
// custom imports
#include "lib/maze/maze.h"
#include "lib/mazeGenerator/mazeGenerator.h"
#include "lib/mazeDisplay/mazeDisplay.h"

#define MIN_SIZE 5
#define MAX_SIZE 20

int main(int argc, char const *argv[])
{
    /** defining random values for the maze generation */
    srand(time(NULL));
    int width = (rand() % MAX_SIZE) + MIN_SIZE;
    int height = (rand() % MAX_SIZE) + MIN_SIZE;
    printf("[INFO] a maze of size %d x %d (width x height) will be generated\n", width, height);

    maze testMaze = generateMaze(width, height);
    displayMaze(testMaze);

    return 0;
}
