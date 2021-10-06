#include "lib/minunit/minunit.h"
// standard import
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
// custom imports
#include "lib/maze/maze.h"
#include "lib/mazeGenerator/mazeGenerator.h"
#include "lib/mazeDisplay/mazeDisplay.h"
#include "lib/mazeHandler/mazeHandler.h"
#include "lib/menu/menu.h"

#define MIN_SIZE 5
#define MAX_SIZE 1000

// mu_check(condition): will pass if the condition is evaluated to true, otherwise it will show the condition as the error message

// mu_fail(message): will fail and show the message

// mu_assert(condition, message): will pass if the condition is true, otherwise it will show the failed condition and the message

// mu_assert_int_eq(expected, result): it will pass if the two numbers are equal or show their values as the error message

// mu_assert_double_eq(expected, result): it will pass if the two values are almost equal or show their values as the error message. The value of MINUNIT_EPSILON sets the threshold to determine if the values are close enough.

// bool mazeIsSolvable(maze m) {
//     // TODO
//     return true;
// }

// MU_TEST(even_size_check) { // TODO
//     int width = MIN_SIZE + rand() %  MAX_SIZE,
//         height = MIN_SIZE + rand() %  MAX_SIZE;
//     maze testMaze = generateMaze(width, height);

//     printf("%d x %d\n", height, width);
//     return mazeIsSolvable(testMaze);
// }

// MU_TEST(odd_size_check) { // TODO
//     int width = MIN_SIZE + rand() % MAX_SIZE,
//         height = MIN_SIZE + rand() % MAX_SIZE;

//     if (width % 2 == 0) width++;
//     if (height % 2 == 0) height++;


//     printf("%d x %d\n", height, width);
//     maze testMaze = generateMaze(width, height);
//     return mazeIsSolvable(testMaze);
// }

// MU_TEST_SUITE(maze_generation_suite) {
//     // for (int i = 0; i < 20; i++)
//     // {
//     //     MU_RUN_TEST(even_size_check);
//     //     MU_RUN_TEST(odd_size_check);
//     // }

//     MU_RUN_TEST(even_size_check);
//     MU_RUN_TEST(odd_size_check);
// }

// MU_TEST_SUITE(maze_handler_suite) {
//     // TODO
// }

int main(int argc, char const *argv[]){
    srand(time(NULL));

    // MU_RUN_SUITE(maze_generation_suite);
    // MU_RUN_SUITE(maze_handler_suite);

    /**
     * dev
    */

    launchMenu();

    /**
        * end dev
    */

    MU_REPORT();
    return MU_EXIT_CODE;
}
