#include "mazeGenerator.h"

maze generateMaze(int width, int height) {
    maze newMaze = {0};

    newMaze.width = width;
    newMaze.height = height;

    newMaze.elements = calloc(width, sizeof(int *));
    for (int i = 0; i < width; i++)
    {
        newMaze.elements[i] = calloc(height, sizeof(int));
    }

    return newMaze;
}