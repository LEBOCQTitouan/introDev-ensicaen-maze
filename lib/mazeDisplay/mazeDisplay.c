#include "mazeDisplay.h"

void displayMaze(maze maze, mazeDisplayType displayType) {
    if (displayType == DETAILLED_MAZE_DISPLAY) {
        printfColored(YELLOW, DEFAULT_COLOR, BOLD, "(%d (width) x %d (height))\n", maze.width, maze.height);
        for (int i = 0; i < maze.height; i++)
        {
            if (i == 0) {
                printf("    ");
                for (int j = 0; j < maze.width; j++) printf("%d", j % 10);
                printf("\n");
                printf("    ");
                for (int j = 0; j < maze.width; j++) printf("v");
                printf("\n");
            }
            printf("%2d> ", i);
            for (int j = 0; j < maze.width; j++)
            {
                if (maze.elements[j][i] == MAZE_ENTRANCE) {
                    printfColored(GREEN, DEFAULT_COLOR, BOLD, "%c", MAZE_ENTRANCE);
                } else if (maze.elements[j][i] == MAZE_EXIT) {
                    printfColored(RED, DEFAULT_COLOR, BOLD, "%c", MAZE_EXIT);
                } else if (maze.elements[j][i] == MAZE_WALL) {
                    printfColored(WHITE, DEFAULT_COLOR, BOLD, "%c", MAZE_WALL);
                } else {
                    printfColored(DEFAULT_COLOR, YELLOW, DEFAULT_STYLE, "%c", MAZE_CORRIDOR);
                }
            }
            printf("\n");
        }
    } else if (displayType == MINIMALIST_MAZE_DISPLAY) {
        for (int i = 0; i < maze.height; i++)
        {
            for (int j = 0; j < maze.width; j++)
            {
                printf("%c", maze.elements[j][i]);
            }
            printf("\n");
        }
    } else {
       for (int i = 0; i < maze.height; i++)
       {
           for (int j = 0; j < maze.width; j++)
            {
                if (maze.elements[j][i] == MAZE_ENTRANCE) {
                    printfColored(GREEN, DEFAULT_COLOR, BOLD, "%c", MAZE_ENTRANCE);
                } else if (maze.elements[j][i] == MAZE_EXIT) {
                    printfColored(RED, DEFAULT_COLOR, BOLD, "%c", MAZE_EXIT);
                } else if (maze.elements[j][i] == MAZE_WALL) {
                    printfColored(WHITE, DEFAULT_COLOR, BOLD, "%c", MAZE_WALL);
                } else {
                    printfColored(DEFAULT_COLOR, YELLOW, DEFAULT_STYLE, "%c", MAZE_CORRIDOR);
                }
            }
            printf("\n");
       }
       
    }
}