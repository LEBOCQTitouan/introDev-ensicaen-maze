#include "mazeDisplay.h"

void displayMaze(maze maze) {
    for (int i = 0; i < maze.width; i++)
    {
        for (int j = 0; j < maze.height; j++)
        {
            if (maze.elements[i][j] == MAZE_ENTRANCE) {
                printfColored(GREEN, DEFAULT_COLOR, BOLD, "%c", MAZE_ENTRANCE);
            } else if (maze.elements[i][j] == MAZE_EXIT) {
                printfColored(RED, DEFAULT_COLOR, BOLD, "%c", MAZE_EXIT);
            } else if (maze.elements[i][j] == MAZE_CORRIDOR) {
                printfColored(WHITE, DEFAULT_COLOR, BOLD, "%c", MAZE_CORRIDOR);
            } else {
                printf("%c", MAZE_CORRIDOR);
            }
        }
        printf("\n");
    }
}