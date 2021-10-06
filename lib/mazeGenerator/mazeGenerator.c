#include "mazeGenerator.h"

matrix generateMazeMatrixNumbers(maze maze) {
    matrix matrixNumbers = {0};
    matrixNumbers.height = (maze.height - 1) / 2 ;
    matrixNumbers.width = (maze.width - 1) / 2;

    matrixNumbers.columns = calloc(maze.width, sizeof(int *));
    for (int i = 0; i < maze.width; i++)
    {
        matrixNumbers.columns[i] = calloc(maze.height, sizeof(int));
    }
    int compteur = 0;
    for (int i = 0; i < matrixNumbers.height; i++)
    {
        for (int j = 0; j < matrixNumbers.width; j++)
        {
            matrixNumbers.columns[j][i] = compteur;
            compteur++;
        }
    }
    
    return matrixNumbers;
}

void printMatrix(matrix matrix) {
    printf("(%d x %d)\n", matrix.width, matrix.height);
    for (int i = 0; i < matrix.height; i++)
    {
        for (int j = 0; j < matrix.width; j++)
        {
            printf("%4d ", matrix.columns[j][i]);
        }
        printf("\n");
    }
    
}

int sumMatrix(matrix m) {
    int sum = 0;
    for (int i = 0; i < m.width; i++)
    {
        for (int j = 0; j < m.height; j++)
        {
            sum += m.columns[i][j];
        }
    }
    return sum;
}

int getMatrixIndexFromMazeIndex(int index) {
    return (index - 1) / 2;
}

int getMazeIndexFromMatrixIndex(int index) {
    return (index - 1) * 2;
}

int compareSidesOfWall(matrix m, wall w) {
    int i1 = getMatrixIndexFromMazeIndex(w.ineighbour1),
        j1 = getMatrixIndexFromMazeIndex(w.jneighbour1),
        i2 = getMatrixIndexFromMazeIndex(w.ineighbour2),
        j2 = getMatrixIndexFromMazeIndex(w.jneighbour2);

    if (m.columns[i1][j1] == m.columns[i2][j2]) return 0;
    else if (m.columns[i1][j1] > m.columns[i2][j2]) return 1;
    else return -1;
}

// TODO -> no auto translate to even 
// TODO -> use odd number instead of storing wall positions + no positionnal matrix
maze generateMaze(int width, int height) {
    if (width % 2 == 0) width--;
    if (height % 2 == 0) height--;
    srand(time(NULL));
    // allocating space for the new maze
    maze newMaze = {0};
    newMaze.width = width;
    newMaze.height = height;
    newMaze.elements = calloc(width, sizeof(int *));
    for (int i = 0; i < width; i++) newMaze.elements[i] = calloc(height, sizeof(int));

    // maze values init
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            newMaze.elements[i][j] = MAZE_WALL;
        }
        
    }
    newMaze.elements[1][0] = MAZE_ENTRANCE;
    newMaze.elements[width - 2][height - 1] = MAZE_EXIT;

    // generating maze walls
    matrix numMatrix = generateMazeMatrixNumbers(newMaze);
    for (int i = 1; i < width; i += 2)
    {
        for (int j = 1; j < height; j += 2)
        {
            newMaze.elements[i][j] = MAZE_CORRIDOR;
        }
    }
    

    int numberOfWalls = 0;
    wall * allWalls = NULL;
    // storing all walls
    for (int j = 1; j < height - 1; j++)
    {
        int isEven = (j % 2 == 0);
        for (int i = isEven ? 1 : 2; i < width - 1; i += 2)
        {
            numberOfWalls++;
            allWalls = realloc(allWalls, sizeof(wall) * numberOfWalls);

            wall newWall = {0};
            if (isEven) {
                newWall.ineighbour1 = i;
                newWall.jneighbour1 = j - 1;
                newWall.ineighbour2 = i;
                newWall.jneighbour2 = j + 1;
            } else {
                newWall.ineighbour1 = i - 1;
                newWall.jneighbour1 = j;
                newWall.ineighbour2 = i + 1;
                newWall.jneighbour2 = j;
            }

            allWalls[numberOfWalls - 1] = newWall;
        }
    }

    int randomWallIndex;
    int compareSidesOfWallValue;
    int numberOfIteration = 0;
    do {
        // pick a wall randomly
        randomWallIndex = rand() % numberOfWalls;
        wall choice = allWalls[randomWallIndex];
        // compare both side of the wall
        compareSidesOfWallValue = compareSidesOfWall(numMatrix, choice);

       if (compareSidesOfWallValue != 0) {
           // matrix edition
            int i1 = getMatrixIndexFromMazeIndex(choice.ineighbour1),
                j1 = getMatrixIndexFromMazeIndex(choice.jneighbour1),
                i2 = getMatrixIndexFromMazeIndex(choice.ineighbour2),
                j2 = getMatrixIndexFromMazeIndex(choice.jneighbour2);
        
            if (compareSidesOfWallValue == 1) { // n1 > n2
                int oldValue = numMatrix.columns[i1][j1];
                numMatrix.columns[i1][j1] = numMatrix.columns[i2][j2];
                for (int i = 0; i < numMatrix.width; i++)
                {
                    for (int j = 0; j < numMatrix.height; j++)
                    {
                        if (numMatrix.columns[i][j] == oldValue) numMatrix.columns[i][j] = numMatrix.columns[i1][j1];
                    }
                }
            } else { // n2 > n1
                int oldValue = numMatrix.columns[i2][j2];
                numMatrix.columns[i2][j2] = numMatrix.columns[i1][j1];
                for (int i = 0; i < numMatrix.width; i++)
                {
                    for (int j = 0; j < numMatrix.height; j++)
                    {
                        if (numMatrix.columns[i][j] == oldValue) numMatrix.columns[i][j] = numMatrix.columns[i1][j1];
                    }
                    
                }
            }
            // make the wall in the maze
            newMaze.elements[(choice.ineighbour1 + choice.ineighbour2) / 2][(choice.jneighbour1 + choice.jneighbour2) / 2] = MAZE_CORRIDOR;
            allWalls[randomWallIndex] = allWalls[numberOfWalls - 1];
            numberOfWalls--;
            numberOfIteration++;
       }
    } while (numberOfIteration < ((newMaze.height/2) * (newMaze.width/2)) - 1);

    return newMaze;
}