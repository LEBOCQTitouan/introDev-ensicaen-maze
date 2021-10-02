#include "maze.h"

mazeElement getMazeElementAt(maze maze, int x, int y) {
    return maze.elements[x][y];
}