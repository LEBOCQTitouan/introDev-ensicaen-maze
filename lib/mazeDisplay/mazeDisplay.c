#include "mazeDisplay.h"

/**
 * displayMazeInfos() display the dim of the maze
*/
void displayMazeInfos(maze m) {
    printfColored(YELLOW, DEFAULT_COLOR, BOLD, "%s (%d x %d)\n", m.name, m.height, m.width);
    printfColored(PURPLE, DEFAULT_COLOR, UNDERLINE, "number of entities : %d\n", m.numberOfEntity);
    printf("\n");

    printfColored(YELLOW, DEFAULT_COLOR, BOLD, "K-");
    printf(" : ");
    printfColored(DEFAULT_COLOR, DEFAULT_COLOR, UNDERLINE, "Key\n");
    printfColored(DEFAULT_COLOR, YELLOW, BOLD, "Tr");
    printf(" : ");
    printfColored(DEFAULT_COLOR, DEFAULT_COLOR, UNDERLINE, "Treasure\n");
    printfColored(DEFAULT_COLOR, RED, BOLD, "Tr");
    printf(" : ");
    printfColored(DEFAULT_COLOR, DEFAULT_COLOR, UNDERLINE, "Trap\n");

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
    default:
        printfColored(RED, DEFAULT, BOLD, "EE");
        break;
    }
}

void displayMaze(maze m) {
    for (int j = 0; j < m.height; j++)
    {
        for (int i = 0; i < m.width; i++) {
            int entityCoord = isEntityCoord(&m, i, j);
            if (entityCoord != -1) {
                displayMazeEntity(m.entities[entityCoord].type);
            } else displayMazeElement(m.elements[i][j]);
        }
        printf("\n");
    }
    displayMazeInfos(m);
    printfColored(YELLOW, DEFAULT_COLOR, UNDERLINE, "entities :\n");
    for (int i = 0; i < m.numberOfEntity && i < 6; i++)
    {
        displayMazeEntity(m.entities[i].type);
        printf(" [%d,%d]\n", m.entities[i].x, m.entities[i].y);
    }
    if (m.numberOfEntity > 6) printfColored(PURPLE, DEFAULT_COLOR, BOLD, "and more ...");
    printf("\n");
}

void displayMazeWithPlayerInfos() {
    printfColored(DEFAULT_COLOR, CYAN, BOLD, "P ");
    printf(" : ");
    printfColored(DEFAULT_COLOR, DEFAULT_COLOR, UNDERLINE, "Player\n");
    
    printf("\n");
}

void displayMazeWithPlayer(mazeHandler handler) {
    maze m = (*handler.maze);
    for (int j = 0; j < m.height; j++)
    {
        for (int i = 0; i < m.width; i++)
        {
            if (i == handler.mazePlayer.position.x && j == handler.mazePlayer.position.y) printfColored(DEFAULT_COLOR, CYAN, BOLD, "Pl");
            else {
                int entityCoord = isEntityCoord(&m, i, j);
                if (entityCoord != -1) {
                    displayMazeEntity(handler.maze->entities[entityCoord].type);
                }
                else displayMazeElement(m.elements[i][j]);
            }
        }
        printf("\n");
    }
    displayMazeInfos(m);
    displayMazeWithPlayerInfos();
}