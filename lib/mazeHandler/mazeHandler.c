#include "mazeHandler.h"

void readWidth(maze * m, FILE * filePtr) {
    fread(&(m->width), sizeof(int), 1, filePtr);
}

void readHeight(maze * m, FILE * filePtr) {
    fread(&(m->height), sizeof(int), 1, filePtr);
}

void readElements(maze * m, FILE * filePtr) {
    m->elements = calloc(m->width, sizeof(int *));
    for (int i = 0; i < m->width; i++)
    {
        m->elements[i] = calloc(m->height, sizeof(int));
        for (int j = 0; j < m->height; j++) {
            fread(&(m->elements[i][j]), sizeof(int), 1, filePtr);
        }
    }
}

maze loadMAze(char * name) {
    maze loadedMaze = {0};
    FILE * mazeFilePtr = NULL;
    char * fileName = calloc(strlen(name) + strlen(MAZE_EXT) + strlen(SAVE_FOLDER), sizeof(char));

    fileName = strcpy(fileName, SAVE_FOLDER);
    fileName = strcat(fileName, name);
    fileName = strcat(fileName, MAZE_EXT);
    
    mazeFilePtr = fopen(fileName, "r");

    readWidth(&loadedMaze, mazeFilePtr);
    readHeight(&loadedMaze, mazeFilePtr);
    readElements(&loadedMaze, mazeFilePtr);
    loadedMaze.name = name;

    free(fileName);
    fclose(mazeFilePtr);
    return loadedMaze;
}

int saveMaze(maze m) {
    FILE * mazeFilePtr = NULL;
    char * fileName = calloc(strlen(m.name) + strlen(MAZE_EXT) + strlen(SAVE_FOLDER), sizeof(char));

    fileName = strcpy(fileName, SAVE_FOLDER);
    fileName = strcat(fileName, m.name);
    fileName = strcat(fileName, MAZE_EXT);

    mazeFilePtr = fopen(fileName, "w");

    int writingState = 0;
    writingState = fwrite(&(m.width), sizeof(int), 1, mazeFilePtr);
    writingState = fwrite(&(m.height), sizeof(int), 1, mazeFilePtr);

    for (int i = 0; i < m.width; i++)
    {
        for (int j = 0; j < m.height; j++)
        {
            writingState = fwrite(&(m.elements[i][j]), sizeof(int), 1, mazeFilePtr);
        }
    }

    int size = strlen(m.name);
    size++;
    writingState = fwrite(&size, sizeof(int), 1, mazeFilePtr);
    writingState = fwrite(m.name, sizeof(char), strlen(m.name), mazeFilePtr);
    writingState = fwrite("\0", sizeof(char), 1, mazeFilePtr);

    free(fileName);
    fclose(mazeFilePtr);
    return 0;
}

mazeHandler initMazeMovement(maze * m) {
    player p = {0};
    p.position.x = 1;
    p.position.y = 0;

    mazeHandler handler = {0};
    handler.mazePlayer = p;
    handler.maze = m;

    return handler;
}

bool movePlayer(mazeHandler * handler, directionPlayer direction) {
    if (isPossibleDirection(*handler, direction)) {
        switch (direction)
        {
        case TOP: 
            handler->mazePlayer.position.y--;
            break;
        case BOTTOM:
            handler->mazePlayer.position.y++;
            break;
        case LEFT:
            handler->mazePlayer.position.x--;
            break;
        case RIGHT:
            handler->mazePlayer.position.x++;
            break;
        default: break;
        }
    }
}

bool isInMazeCoord(maze m, int x, int y) {
    return ( x >= 0 && y >= 0 && x < m.width && y < m.height );
}

bool isPossibleDirection(mazeHandler handler, directionPlayer direction) {
    switch (direction)
    {
    case TOP: return (
            isInMazeCoord(*handler.maze, handler.mazePlayer.position.x, handler.mazePlayer.position.y - 1)
            && handler.maze->elements[handler.mazePlayer.position.x][handler.mazePlayer.position.y - 1] != MAZE_WALL
        );
    case BOTTOM: return (
            isInMazeCoord(*handler.maze, handler.mazePlayer.position.x, handler.mazePlayer.position.y + 1)
            && handler.maze->elements[handler.mazePlayer.position.x][handler.mazePlayer.position.y + 1] != MAZE_WALL
        );
    case LEFT: return (
            isInMazeCoord(*handler.maze, handler.mazePlayer.position.x - 1, handler.mazePlayer.position.y)
            && handler.maze->elements[handler.mazePlayer.position.x - 1][handler.mazePlayer.position.y] != MAZE_WALL
        );
    case RIGHT: return (
            isInMazeCoord(*handler.maze, handler.mazePlayer.position.x + 1, handler.mazePlayer.position.y)
            && handler.maze->elements[handler.mazePlayer.position.x + 1][handler.mazePlayer.position.y] != MAZE_WALL
        );
    
    default: return false;
    }
}

bool compareMaze(maze m1, maze m2) {
    if (m1.height != m2.height) return false;
    if (m1.width != m2.width) return false;
    for (int i = 0; i < m1.width; i++)
    {
        for (int j = 0; j < m1.height; j++)
        {
            if (m1.elements[i][j] != m2.elements[i][j]) return false;
        }
    }
    return true;
}