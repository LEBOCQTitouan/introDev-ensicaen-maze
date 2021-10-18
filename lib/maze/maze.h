#ifndef MAZE_H
#define MAZE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * mazeElement is an enum defining the values of the different maze elements
*/
typedef enum {
    MAZE_ENTRANCE,
    MAZE_WALL,
    MAZE_CORRIDOR,
    MAZE_EXIT
} mazeElement;

/**
 * entityType is an enum used to define the type of the entity which are on the maze (hardcore mode)
*/
typedef enum {
    KEY,
    TREASURE,
    TRAP,
    GHOST,
    TROLL
} entityType;

typedef enum {
    MOVE_UP = 0,
    MOVE_DOWN,
    MOVE_RIGHT,
    MOVE_LEFT
} movingDirection;

typedef struct entity_ entity;
typedef struct maze_ maze;

typedef void (*mobility_action)(int xPlayer, int yPlayer, entity * entity, maze * m);

/**
 * entity is a struct used to describe the entities in the maze (hardcore mode)
*/
struct entity_
{
    int x, y;
    entityType type;
    bool isAlive;

    mobility_action move;
};


/**
 * maze (struct mazes) is the structure holding all maze data
*/
struct maze_
{
    mazeElement ** elements;

    int width;
    int height;

    char * name;

    entity * entities;
    int numberOfEntity;
    bool isUnlocked;
};


int getEntityPenaltyValue(entityType type);

mobility_action getMobilityAction(entityType type);

#include "../mazeHandler/mazeHandler.h"

#endif /* MAZE_H */
