#include "mazeDisplay.h"

/**
 * displayMazeInfos() display the dim of the maze
*/
void displayMazeInfos(maze m) {
    printfColored(YELLOW, DEFAULT_COLOR, BOLD, "%s (%d x %d)\n", m.name, m.height, m.width);
    printf("\n");
}

void displayMazeElement(mazeElement element) {
    switch (element)
    {
    case MAZE_ENTRANCE:
        printfColored(WHITE, GREEN, BOLD, "--", MAZE_ENTRANCE, MAZE_ENTRANCE);
        break;
    case MAZE_EXIT:
        printfColored(WHITE, RED, BOLD, "oo", MAZE_EXIT, MAZE_EXIT);
        break;
    case MAZE_CORRIDOR:
        printfColored(BLACK, BLACK, BOLD, "  ", MAZE_CORRIDOR, MAZE_CORRIDOR);
        break;
    case MAZE_WALL:
        printfColored(WHITE, WHITE, BOLD, "##", MAZE_WALL, MAZE_WALL);
        break;
    default :
        printfColored(RED, DEFAULT, BOLD, "EE");
        break;
    }
}

/**
 * the isEntityCoord() function will return the index of the entity at x y or -1 if no entity
*/
int isEntityCoord(maze * m, int x, int y) {
    for (int i = 0; i < m->numberOfEntity; i++)
    {
        if (m->entities[i].x == x && m->entities[y].y == y) return i;
    }
}

void displayMaze(maze m) {
    for (int j = 0; j < m.height; j++)
    {
        for (int i = 0; i < m.width; i++) displayMazeElement(m.elements[i][j]);
        printf("\n");
    }
    displayMazeInfos(m);
}

void displayMazeWithPlayerInfos() {
    printfColored(DEFAULT_COLOR, CYAN, BOLD, "P");
    printfColored(DEFAULT_COLOR, DEFAULT_COLOR, BOLD, " : Player\n");
    printf("\n");
}

void displayMazeEntity(entityType entity) {
    switch (entity)
    {
    case KEY:
        printfColored(YELLOW, DEFAULT_COLOR, BOLD, "K-");
        break;
    case TREASURE:
        printfColored(DEFAULT_COLOR, YELLOW, BOLD, "Tr");
        break;
    case TRAP:
        printfColored(DEFAULT_COLOR, RED, BOLD, "Tr");
        break;
    default: break;
    }
}

void displayMazeWithPlayer(mazeHandler handler) {
    maze m = (*handler.maze);
    for (int j = 0; j < m.height; j++)
    {
        for (int i = 0; i < m.width; i++)
        {
            int  entityCoord = -1;
            if (i == handler.mazePlayer.position.x && j == handler.mazePlayer.position.y) printfColored(DEFAULT_COLOR, CYAN, BOLD, "Pl");
            else if ((entityCoord = isEntityCoord(handler.maze, i, j)) != -1) {
                displayMazeEntity(handler.maze->entities[entityCoord].type);
            } else displayMazeElement(m.elements[i][j]);
        }
        printf("\n");
    }
    displayMazeInfos(m);
    displayMazeWithPlayerInfos();
}