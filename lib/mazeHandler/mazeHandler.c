#include "mazeHandler.h"

// TODO reset end game

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

entity loadEntity(FILE * f) {
    entity en = {0};

    fread(&(en.x), sizeof(int), 1, f);
    fread(&(en.y), sizeof(int), 1, f);
    fread(&(en.type), sizeof(entityType), 1, f);

    en.isAlive = true;
    en.move = getMobilityAction(en.type);

    return en;
}

void loadEntities(maze * m, FILE * f) {
    int size;
    fread(&size, sizeof(int), 1, f);
    entity * entities = calloc(size, sizeof(entity));
    for (int i = 0; i < size; i++)
    {
        entities[i] = loadEntity(f);
    }
    
    m->numberOfEntity = size;
    m->entities = entities;
}

// TODO edit for entity save
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
    loadEntities(&loadedMaze, mazeFilePtr);
    loadedMaze.name = name;

    free(fileName);
    fclose(mazeFilePtr);
    return loadedMaze;
}


int saveWidth(int width, FILE * f) {
    return fwrite(&width, sizeof(int), 1, f);
}

int saveHeight(int height, FILE * f) {
    return fwrite(&height, sizeof(int), 1, f);
}

int saveElements(int width, int height, mazeElement ** elements, FILE * f) {
    int writingState = 0;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            writingState = fwrite(&(elements[i][j]), sizeof(int), 1, f);
        }
    }
    return writingState;
}

// int x, y;
// entityType type;
// bool isAlive;
int saveEntity(entity en, FILE * f) {
    int writtingState = 0;
    writtingState = fwrite(&(en.x), sizeof(int), 1, f);
    writtingState = fwrite(&(en.y), sizeof(int), 1, f);
    writtingState = fwrite(&(en.type), sizeof(entityType), 1, f);
    return writtingState;
}

int saveEntities(int n, entity * entities, FILE * f) {
    int writingState = 0;
    writingState = fwrite(&n, sizeof(int), 1, f);
    for (int i = 0; i < n; i++)
    {
        entity * current = &(entities[i]);
        writingState = saveEntity(*current, f);
    }
    return writingState;
}

int saveMaze(maze m) {
    FILE * mazeFilePtr = NULL;
    char * fileName = calloc(strlen(m.name) + strlen(MAZE_EXT) + strlen(SAVE_FOLDER), sizeof(char));

    fileName = strcpy(fileName, SAVE_FOLDER);
    fileName = strcat(fileName, m.name);
    fileName = strcat(fileName, MAZE_EXT);
    mazeFilePtr = fopen(fileName, "w");
    free(fileName);

    int writingState = 0;

    writingState = saveWidth(m.width, mazeFilePtr);
    writingState = saveHeight(m.height, mazeFilePtr);
    writingState = saveElements(m.width, m.height, m.elements, mazeFilePtr);
    writingState = saveEntities(m.numberOfEntity, m.entities, mazeFilePtr);

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

    handler.score = 1000;

    return handler;
}

void updateScore(mazeHandler * handler) {
    // check if an entity is on the player
    for (int i = 0; i < handler->maze->numberOfEntity; i++)
    {
        entity current = handler->maze->entities[i];
        if (
            current.x == handler->mazePlayer.position.x && current.y == handler->mazePlayer.position.y
        ) {
            handler->score += getEntityPenaltyValue(current.type);
            handler->maze->entities[i].isAlive = false;
        }
    }
    // score update (move)
    handler->score -= 10;
}

void updateEntities(mazeHandler * handler) {
    for (int i = 0; i < handler->maze->numberOfEntity; i++)
    {
        entity * current = &(handler->maze->entities[i]);
        int x, y;
        x = handler->mazePlayer.position.x;
        y = handler->mazePlayer.position.y;
        current->move(x, y, &(handler->maze->entities[i]), handler->maze);
    }
    
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
        updateEntities(handler);
        updateScore(handler);
    }
}

bool isInMazeCoord(maze m, int x, int y) {
    return ( x >= 0 && y >= 0 && x < m.width && y < m.height );
}

// TODO cleaner boolean expr
bool isPossibleDirection(mazeHandler handler, directionPlayer direction) {
    return isPossibleDirectionFromCoords(handler.maze, handler.mazePlayer.position.x, handler.mazePlayer.position.y, direction);
}

bool isPossibleDirectionFromCoords(maze * m, int x, int y, directionPlayer direction) {
    switch (direction)
    {
    case TOP: return (
            isInMazeCoord(*m, x, y - 1)
            && (
                m->elements[x][y - 1] != MAZE_WALL && (
                    m->elements[x][y - 1] != MAZE_EXIT
                    || (
                        m->elements[x][y - 1] == MAZE_EXIT
                        && m->isUnlocked
                    )
                )
            )
        );
    case BOTTOM: return (
            isInMazeCoord(*m, x, y + 1)
            && (
                m->elements[x][y + 1] != MAZE_WALL
                && (
                    m->elements[x][y + 1] != MAZE_EXIT
                    || (
                        m->elements[x][y + 1] == MAZE_EXIT
                        && m->isUnlocked
                    )
                )
            )
        );
    case LEFT: return (
            isInMazeCoord(*m, x - 1, y)
            && (
                m->elements[x - 1][y] != MAZE_WALL
                && (
                    m->elements[x - 1][y] != MAZE_EXIT
                    || (
                        m->elements[x -1][y] != MAZE_WALL
                        && m->isUnlocked
                    )
                )
            )
        );
    case RIGHT: return (
            isInMazeCoord(*m, x + 1, y)
            && (
                m->elements[x + 1][y] != MAZE_WALL
                && (
                    m->elements[x + 1][y] != MAZE_EXIT
                    || (
                        m->elements[x + 1][y] == MAZE_EXIT
                        && m->isUnlocked
                    )
                )
            )
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

int isEntityCoord(maze * m, int x, int y) {
    for (int i = 0; i < (*m).numberOfEntity; i++)
    {
        if (m->entities[i].x == x && m->entities[i].y == y) return i;
    }
    return -1;
}