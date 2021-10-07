#include "mazeDisplay.h"

/**
 * displayMazeInfos() display the dim of the maze
*/
void displayMazeInfos(maze m) {
    printfColored(YELLOW, DEFAULT_COLOR, BOLD, "(%d x %d)\n", m.height, m.width);
}

void displayMaze(maze m) {
    for (int j = 0; j < m.height; j++)
    {
        for (int i = 0; i < m.width; i++)
        {
            
            if (m.elements[i][j] == MAZE_ENTRANCE) printfColored(WHITE, GREEN, BOLD, "%c%c", MAZE_ENTRANCE, MAZE_ENTRANCE);
            else if (m.elements[i][j] == MAZE_EXIT) printfColored(WHITE, RED, BOLD, "%c%c", MAZE_EXIT, MAZE_EXIT);
            else if (m.elements[i][j] == MAZE_CORRIDOR) printfColored(BLACK, BLACK, BOLD, "%c%c", MAZE_CORRIDOR, MAZE_CORRIDOR);
            else printfColored(WHITE, WHITE, BOLD, "%c%c", MAZE_WALL, MAZE_WALL);
        }
        printf("\n");
    }
    displayMazeInfos(m);
}

void displayMazeWithPlayer(mazeHandler handler) {
    maze m = (*handler.maze);
    for (int j = 0; j < m.height; j++)
    {
        for (int i = 0; i < m.width; i++)
        {
            if (i == handler.mazePlayer.position.x && j == handler.mazePlayer.position.y) printfColored(DEFAULT_COLOR, CYAN, BOLD, "Pl");
            else if (m.elements[i][j] == MAZE_ENTRANCE) printfColored(WHITE, GREEN, BOLD, "%c%c", MAZE_ENTRANCE, MAZE_ENTRANCE);
            else if (m.elements[i][j] == MAZE_EXIT) printfColored(WHITE, RED, BOLD, "%c%c", MAZE_EXIT, MAZE_EXIT);
            else if (m.elements[i][j] == MAZE_CORRIDOR) printfColored(BLACK, BLACK, BOLD, "%c%c", MAZE_CORRIDOR, MAZE_CORRIDOR);
            else printfColored(WHITE, WHITE, BOLD, "%c%c", MAZE_WALL, MAZE_WALL);
        }
        printf("\n");
    }
    displayMazeInfos(m);
    printf("\n");
    printfColored(DEFAULT_COLOR, CYAN, BOLD, "P");
    printfColored(DEFAULT_COLOR, DEFAULT_COLOR, BOLD, " : Player\n");
}