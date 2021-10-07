#ifndef MAZE_H
#define MAZE_H

/**
 * mazeElement is an enum defining the values of the different maze elements
*/
typedef enum {
    MAZE_ENTRANCE   = 'O',
    MAZE_WALL       = '#',
    MAZE_CORRIDOR   = ' ',
    MAZE_EXIT       = '-'
} mazeElement;

/**
 * entityType is an enum used to define the type of the entity which are on the maze (hardcore mode)
*/
typedef enum {
    KEY,
    TREASURE,
    TRAP
} entityType;

/**
 * entity is a struct used to describe the entities in the maze (hardcore mode)
*/
typedef struct {
    int x, y;
    entityType type;
} entity;

/**
 * maze (struct mazes) is the structure holding all maze data
*/
typedef struct mazes
{
    mazeElement ** elements;

    int width;
    int height;

    entity * entities;
    int numberOfEntity;
} maze;

#endif //MAZE_H
