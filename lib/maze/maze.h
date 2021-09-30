#ifndef MAZE_H
#define MAZE_H

typedef enum {
    MAZE_ENTRANCE   = 'O',
    MAZE_WALL       = '#',
    MAZE_CORRIDOR   = ' ',
    MAZE_EXIT       = '-'
} mazeElement;

typedef struct mazes
{
    mazeElement ** elements;

    int width;
    int height;
} maze;

#endif //MAZE_H
