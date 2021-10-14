#include "test.h"

void gen_random(char *s, const int len) {
    static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

// mu_check(condition): will pass if the condition is evaluated to true, otherwise it will show the condition as the error message

// mu_fail(message): will fail and show the message

// mu_assert(condition, message): will pass if the condition is true, otherwise it will show the failed condition and the message

// mu_assert_int_eq(expected, result): it will pass if the two numbers are equal or show their values as the error message

// mu_assert_double_eq(expected, result): it will pass if the two values are almost equal or show their values as the error message. The value of MINUNIT_EPSILON sets the threshold to determine if the values are close enough.

MU_TEST(even_size_check) { // TODO
    int width = MIN_SIZE + rand() %  MAX_SIZE,
        height = MIN_SIZE + rand() %  MAX_SIZE;
    maze testMaze = generateMaze(width, height, "test", NORMAL_MODE);
    mu_check(testMaze.width % 2 == 1 && testMaze.height % 2 == 1);
}

MU_TEST(odd_size_check) { // TODO
    int width = MIN_SIZE + rand() % MAX_SIZE,
        height = MIN_SIZE + rand() % MAX_SIZE;

    if (width % 2 == 0) width++;
    if (height % 2 == 0) height++;

    maze testMaze = generateMaze(width, height, "test", NORMAL_MODE);
    mu_check(testMaze.width % 2 == 1 && testMaze.height % 2 == 1);
}

MU_TEST(solve_maze_check) {
    // TODO solve maze
}

MU_TEST_SUITE(maze_generation_suite) {
    MU_RUN_TEST(even_size_check);
    MU_RUN_TEST(odd_size_check);
    MU_RUN_TEST(solve_maze_check);
}

// TODO entities
MU_TEST(save_maze_check) {
    char filename[20] = {0};
    gen_random(filename, 20);
    
    maze checkMaze = generateMaze(rand() % 100 + 5, rand() % 100 + 5, filename, NORMAL_MODE);

    // int saveStatus = saveMaze(checkMaze);
    saveMaze(checkMaze);

    maze checkMazeLoad = loadMAze(filename);

    if (strcmp(checkMaze.name, checkMazeLoad.name) != 0) mu_fail("Loaded maze name is different from the initial maze");
    if (compareMaze(checkMaze, checkMazeLoad) != true) mu_fail("Loaded maze elements are not equal to starting maze elements");
    if (checkMaze.numberOfEntity != checkMazeLoad.numberOfEntity) {
        mu_fail("Loaded maze number of entity differs from initial maze");
    } else {
        for (int i = 0; i < checkMaze.numberOfEntity; i++)
        {
            if (
                checkMaze.entities[i].x != checkMazeLoad.entities[i].x
                || checkMaze.entities[i].y != checkMazeLoad.entities[i].y
                || checkMaze.entities[i].type != checkMazeLoad.entities[i].type
            ) {
                char * message = calloc(100, sizeof(char)); // danger if too big number
                sprintf(message, "Loaded maze %d entity differs from initial maze", i);
                mu_fail(message);
            }
        }
    }

    char * pathToFile = NULL;
    pathToFile = calloc(strlen(filename) + strlen(SAVE_FOLDER) + strlen(MAZE_EXT) + 1, sizeof(char));
    pathToFile[0] = '\0';
    strcat(pathToFile, SAVE_FOLDER);
    strcat(pathToFile, filename);
    strcat(pathToFile, MAZE_EXT);

    remove(pathToFile);
}

// TODO check if move out of the maze is possible
// TODO check move
MU_TEST_SUITE(maze_handler_suite) {
    MU_RUN_TEST(save_maze_check);
}


int main(int argc, char const *argv[]){
    srand(time(NULL));

    MU_RUN_SUITE(maze_generation_suite);
    MU_RUN_SUITE(maze_handler_suite);

    maze test = generateMaze(50, 50, "test", HARDCORE_MODE);
    printf("\n");
    displayMaze(test);

    MU_REPORT();
    return MU_EXIT_CODE;
}
