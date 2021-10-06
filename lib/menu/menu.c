#include "menu.h"

menuType currentMenu = SELECTION;
char * errorMessage = NULL;
maze menuMaze = {0};
// selection
menuType selectionChoice = CREATE_MAZE;
// create maze
#define MAZE_CREATION_FOCUS_MAX 5
int mazeCreationWidth = 5;
int mazeCreationHeight = 5;
int mazeCreationCurrentFocus = 0;
maze * newMaze = NULL;
// load maze
int loadMazeNumberOfFile = 0;
char ** fileNames = NULL;
int loadMazeCurrentFocus = 0;
// play

/** selection event listenner */
void selectionDown() {
    selectionChoice++;
    if (selectionChoice > EXIT) selectionChoice = CREATE_MAZE;
}

void selectionUp() {
    selectionChoice--;
    if (selectionChoice <= SELECTION) selectionChoice = EXIT;
}

void enterSelection() {
    changeMenu(selectionChoice);
}

/** create maze event listener */
void createDown() {
    mazeCreationCurrentFocus++;
    mazeCreationCurrentFocus = mazeCreationCurrentFocus % MAZE_CREATION_FOCUS_MAX;
}
void createUp() {
    mazeCreationCurrentFocus--;
    if (mazeCreationCurrentFocus < 0) mazeCreationCurrentFocus = MAZE_CREATION_FOCUS_MAX - 1;
}
void createLeft() {
    switch (mazeCreationCurrentFocus)
    {
    case 0:
        if (mazeCreationWidth < 6) { break; } // TODO error
        *newMaze = generateMaze(mazeCreationWidth, mazeCreationHeight);
        mazeCreationWidth-=2;
        break;
    case 1:
        if (mazeCreationHeight < 6) { break; } // TODO error
        mazeCreationHeight-=2;
        *newMaze = generateMaze(mazeCreationWidth, mazeCreationHeight);
        break;
    default: break;
    }
}

void createRight() {
    switch (mazeCreationCurrentFocus)
    {
    case 0:
        mazeCreationWidth+=2;
        *newMaze = generateMaze(mazeCreationWidth, mazeCreationHeight);
        break;
    case 1:
        mazeCreationHeight+=2;
        *newMaze = generateMaze(mazeCreationWidth, mazeCreationHeight);
        break;
    default: break;
    }}
void createEnter() {
    switch (mazeCreationCurrentFocus)
    {
    case 2:
        *newMaze = generateMaze(mazeCreationWidth, mazeCreationHeight);
        break;
    case 3:
        changeMenu(SELECTION);
        menuMaze = *newMaze;
        newMaze = NULL;
    case 4:
        changeMenu(SELECTION);
        break;
    default: break;
    }
}

void changeMenu(menuType newMenu) {
    currentMenu = newMenu;
    rawTerminal_action  arrowDown = {0},
                        arrowUp = {0},
                        arrowLeft = {0},
                        arrowRight = {0},
                        enterPressed = {0};

    arrowDown.type = VOID_FUNCTION;
    arrowUp.type = VOID_FUNCTION;
    arrowLeft.type = VOID_FUNCTION;
    arrowRight.type = VOID_FUNCTION;
    enterPressed.type = VOID_FUNCTION;

    switch (newMenu)
    {
    case SELECTION:
        arrowDown.func.void_function = &selectionDown;
        arrowUp.func.void_function = &selectionUp;
        enterPressed.func.void_function = &enterSelection;

        assignActionToKeyBoardInput(INPUT_ARROW_DOWN, &arrowDown);
        assignActionToKeyBoardInput(INPUT_ARROW_UP, &arrowUp);
        assignActionToKeyBoardInput(INPUT_ENTER, &enterPressed);

        selectionChoice = CREATE_MAZE;
        break;
    
    case CREATE_MAZE:
        newMaze = calloc(1, sizeof(maze));
        mazeCreationCurrentFocus = 0;

        arrowDown.func.void_function = &createDown;
        arrowUp.func.void_function = &createUp;
        arrowLeft.func.void_function = &createLeft;
        arrowRight.func.void_function = &createRight;
        enterPressed.func.void_function = &createEnter;

        assignActionToKeyBoardInput(INPUT_ARROW_DOWN, &arrowDown);
        assignActionToKeyBoardInput(INPUT_ARROW_UP, &arrowUp);
        assignActionToKeyBoardInput(INPUT_ARROW_RIGHT, &arrowRight);
        assignActionToKeyBoardInput(INPUT_ARROW_LEFT, &arrowLeft);
        assignActionToKeyBoardInput(INPUT_ENTER, &enterPressed);

        *newMaze = generateMaze(mazeCreationWidth, mazeCreationHeight);
        break;
    default: break;
    }
}

void display() {
    if (errorMessage != NULL) printfColored(WHITE, RED, BOLD, "/!\\ %s /!\\ \n", errorMessage);
    switch (currentMenu)
    {
    case SELECTION:
        printfColored(BLACK, WHITE, UNDERLINE, "- SELECTION -\n");
        if (selectionChoice == CREATE_MAZE) {
            printfColored(BLACK, WHITE, BOLD, "Create maze\n");
        } else printfColored(WHITE, DEFAULT_COLOR, BOLD, "Create maze\n");
        if (selectionChoice == LOAD_MAZE) {
            printfColored(BLACK, WHITE, BOLD, "Load maze\n");
        } else printfColored(WHITE, DEFAULT_COLOR, BOLD, "Load maze\n");
        if (selectionChoice == PLAY) {
            printfColored(BLACK, WHITE, BOLD, "Play\n");
        } else printfColored(WHITE, DEFAULT_COLOR, BOLD, "Play\n");
        if (selectionChoice == EXIT) {
            printfColored(BLACK, WHITE, BOLD, "Exit\n");
        } else printfColored(WHITE, DEFAULT_COLOR, BOLD, "Exit\n");
        break;
    case CREATE_MAZE:
        printfColored(BLACK, WHITE, UNDERLINE, "- CREATE MAZE -\n");
        if (newMaze != NULL) displayMaze(*newMaze);
        switch (mazeCreationCurrentFocus)
        {
        case 0:
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "width :\t\t");
            printfColored(YELLOW, DEFAULT_COLOR, BOLD, "<");
            printf(" ");
            printfColored(BLACK, WHITE, BOLD, "%d", mazeCreationWidth);
            printf(" ");
            printfColored(YELLOW, DEFAULT_COLOR, BOLD, ">\n");
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "height:\t\t< %d >\n", mazeCreationHeight);
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "generate\n");
            printfColored(GREEN, DEFAULT_COLOR, BOLD, "load this maze\n");
            printfColored(RED, DEFAULT_COLOR, BOLD, "Back to menu\n");
            break;
        case 1: 
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "width :\t\t< %d >\n", mazeCreationWidth);
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "height:\t\t");
            printfColored(YELLOW, DEFAULT_COLOR, BOLD, "<");
            printf(" ");
            printfColored(BLACK, WHITE, BOLD, "%d", mazeCreationHeight);
            printf(" ");
            printfColored(YELLOW, DEFAULT_COLOR, BOLD, ">\n");
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "generate\n");
            printfColored(GREEN, DEFAULT_COLOR, BOLD, "load this maze\n");
            printfColored(RED, DEFAULT_COLOR, BOLD, "Back to menu\n");
            break; 
        case 2:
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "width :\t\t< %d >\n", mazeCreationWidth);
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "height:\t\t< %d >\n", mazeCreationHeight);
            printfColored(BLACK, WHITE, BOLD, "generate\n");
            printfColored(GREEN, DEFAULT_COLOR, BOLD, "load this maze\n");
            printfColored(RED, DEFAULT_COLOR, BOLD, "Back to menu\n");
            break;
        case 3:
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "width :\t\t< %d >\n", mazeCreationWidth);
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "height:\t\t< %d >\n", mazeCreationHeight);
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "generate\n");
            printfColored(WHITE, GREEN, BOLD, "load this maze\n");
            printfColored(RED, DEFAULT_COLOR, BOLD, "Back to menu\n");
            break;
        case 4:
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "width :\t\t< %d >\n", mazeCreationWidth);
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "height:\t\t< %d >\n", mazeCreationHeight);
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "generate\n");
            printfColored(GREEN, DEFAULT_COLOR, BOLD, "load this maze\n");
            printfColored(WHITE, RED, BOLD, "Back to menu\n");
            break;
        default: break;
        }
        break;
    case LOAD_MAZE:
        printfColored(BLACK, WHITE, UNDERLINE, "- LOAD MAZE -\n");
        break;
    case PLAY:
        printfColored(BLACK, WHITE, UNDERLINE, "- PLAY -\n");
        break;
    default: break;
    }
}

void launchMenu() {

    enableTerminalRawMode();
    // setup for the menu
    changeMenu(SELECTION);
    // action in the terminal
    while (currentMenu != EXIT)
    {
        // clean the termial for new display
        home();
        clearAllTerminal();

        display();

        char * input = HandleRawModeKeyboard();
        if (errorMessage != NULL) {
            free(errorMessage);
            errorMessage = NULL;
        }
    }

    disableTerminalRawMode();
}