#include "mazeGenerator.h"

/**
 * generateMazeMatrixNumbers() will generate the matrix corresponding to the given maze
 * 
 * @param maze the maze you want to create the matrix from
 * 
 * @return the generated matrix
*/
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

/**
 * printMatrix() will display the given matrix in the terminal
 * 
 * @param matrix the matrix you want to display
*/
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

/**
 * sumMatrix will return the sum of all the element in the given matrix
 * 
 * @param m the matrix you want to sum
 * 
 * @return the sum of the given matrix
*/
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

/**
 * getMatrixIndexFromMazeIndex() will translate a matrix index to a maze index
 * 
 * @param index the index you want to translate
 * 
 * @return the translated value
*/
int getMatrixIndexFromMazeIndex(int index) {
    return (index - 1) / 2;
}

/**
 * getMazeIndexFromMatrixIndex() will translate a maze index to a matrix index
 * 
 * @param index the index you want to translate
 * 
 * @return the translated value
*/
int getMazeIndexFromMatrixIndex(int index) {
    return (index - 1) * 2;
}

/**
 * compareSidesOfWall() will compare the size of two walls given in arg.
 * 
 * @param m the matrix used for comparaison of the walls
 * @param w the wall which you want to compare from
 * 
 * @return 0 if both sides are equal, 1 if the first neighbour of the wall is superior and -1 otherwise (the second neighbour is greater)
*/
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
// TODO -> optimisation propagation
/**
 * initMaze() will init a maze of the given size for the random generation algorythm
 * 
 * @param width the width of the maze you want to init
 * @param height the height of the maze you want to init
 * 
 * @return the initialized maze
*/
maze initMaze(int width, int height) {
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

    for (int i = 1; i < width; i += 2)
    {
        for (int j = 1; j < height; j += 2)
        {
            newMaze.elements[i][j] = MAZE_CORRIDOR;
        }
    }

    return newMaze;
}

/**
 * initAllWalls() will fill the array of wall given in the given bound (maze size)
 * 
 * @param walls the array which will be filled
 * @param width the width of the maze
 * @param height the height of the maze
 * 
 * @return the number of walls in the array
*/
int initAllWAlls(wall ** walls, int width, int height) {
    int numberOfWalls = 0; // the total number of walls
    wall * allWalls = NULL;
    for (int j = 1; j < height - 1; j++)
    {
        bool isEven = (j % 2 == 0); // boolean used to know if the current index is an even index
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
    (*walls) = allWalls;
    return numberOfWalls;
}

maze generateMaze(int width, int height) {
    if (width % 2 == 0) width--;
    if (height % 2 == 0) height--;
    srand(time(NULL));

    // allocating space for the new maze
    maze newMaze = initMaze(width, height);

    // generating maze walls
    matrix numMatrix = generateMazeMatrixNumbers(newMaze);
    wall * allWalls = NULL;
    int numberOfWalls = initAllWAlls(&allWalls, width, height);

    // creating the maze by destructing walls
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