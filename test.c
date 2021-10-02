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

/*
- check height / width => != 0 / between MIN and MAX size
- check elements => NULL ? / size = real size ?
*/

int main(int argc, char const *argv[])
{
    /** Etape 1 */
    maze test11x25maze = generateMaze(25, 11);
    displayMaze(test11x25maze, DETAILLED_MAZE_DISPLAY);

    /** defining random values for the random maze generation */
    srand(time(NULL));
    int width = (rand() % MAX_SIZE) + MIN_SIZE;
    int height = (rand() % MAX_SIZE) + MIN_SIZE;

    maze testRandomMaze = generateMaze(width, height);
    displayMaze(testRandomMaze, DETAILLED_MAZE_DISPLAY);

    return 0;
}
