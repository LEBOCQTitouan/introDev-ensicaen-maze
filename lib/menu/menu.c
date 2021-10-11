#include "menu.h"

// TODO => error in save (while saving)

menuType currentMenu = SELECTION; // the menu currently in use
// char * errorMessage = NULL; // TODO
maze menuMaze = {0}; // the maze in use in the function
// selection
menuType selectionChoice = CREATE_MAZE;
// create maze
#define MAZE_CREATION_FOCUS_MAX 5   // the maximal index (menu elements) in the maze creation
int mazeCreationWidth = 5;          // the width of the maze which will be created
int mazeCreationHeight = 5;         // the height of the meaz which will be created
int mazeCreationCurrentFocus = 0;   // the current menu item selected
maze * newMaze = NULL;              // the maze created in the creation menu
// load maze
int loadMazeNumberOfFile = 0;       // the number of save file found
struct dirent * saveFiles = NULL;   // the array of save file loaded
int * saveFileSkipIndex = NULL;     // the index of the file which does not correspond to a save
int skipIndexSize = 0;              // the size of the array of file to skip
int loadMazeCurrentFocus = 0;       // the current menu item selected
// play
mazeHandler menuMazeHandler = {0};  // the maze handler used to play
// save maze
bool wouldSaveMaze = true;          // bool used to ask if the user want to save the current maze
char * saveName = NULL;             // the name of the save which will be created

/**
 * The following funtions are used in the event listener bind to each key (keyboard)
*/
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
        mazeCreationWidth-=2;
        *newMaze = generateMaze(mazeCreationWidth, mazeCreationHeight);
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

/** load maze event listenner */
// TODO - filename [suppr] del file
// TODO - dnt print all files but portion with an indicator showing there are more
int isInSkipIndex(int index) {
    for (int i = 0; i < skipIndexSize; i++)
    {
        if (saveFileSkipIndex[i] == index) return 1;
    }
    return 0;
}

void loadDown() {
    do
    {
        loadMazeCurrentFocus++;
        loadMazeCurrentFocus = loadMazeCurrentFocus % (loadMazeNumberOfFile + 1);
    } while (isInSkipIndex(loadMazeCurrentFocus));
}

void loadUp() {
    do
    {
        loadMazeCurrentFocus--;
        if (loadMazeCurrentFocus < 0) loadMazeCurrentFocus = loadMazeNumberOfFile;
    } while (isInSkipIndex(loadMazeCurrentFocus));
}

void loadEnter() {
    if (loadMazeCurrentFocus == loadMazeNumberOfFile) {
        loadMazeNumberOfFile = 0;
        saveFiles = NULL;
        saveFileSkipIndex = NULL;
        skipIndexSize = 0;
        loadMazeCurrentFocus = 0;

        changeMenu(SELECTION);
    } else {
        char * fileNameWithEXT = saveFiles[loadMazeCurrentFocus].d_name;
        int size = sizeof(char) * (strlen(fileNameWithEXT) - strlen(MAZE_EXT));
        char * fileName = calloc(size, sizeof(char));
        memcpy(fileName, fileNameWithEXT, size);
        menuMaze = loadMAze(fileName);

        changeMenu(SELECTION);
    }
}

/** play event listenner*/
void checkEndGame() {
    if (menuMazeHandler.maze->elements[menuMazeHandler.mazePlayer.position.x][menuMazeHandler.mazePlayer.position.y] == MAZE_EXIT) {
        changeMenu(SELECTION);
    }
}

void playUp() {
    movePlayer(&menuMazeHandler, TOP);
    checkEndGame();
}

void playDown() {
    movePlayer(&menuMazeHandler, BOTTOM);
    checkEndGame();
}

void playLeft() {
    movePlayer(&menuMazeHandler, LEFT);
    checkEndGame();
}

void playRight() {
    movePlayer(&menuMazeHandler, RIGHT);
    checkEndGame();
}

void playEsc() {
    changeMenu(SAVE_MAZE_CHOICE);
}

/** save event listenner */
void saveChoiceLeft() {
    wouldSaveMaze = !wouldSaveMaze;
}

void saveChoiceRight() {
    wouldSaveMaze = !wouldSaveMaze;
}

void saveChoiceEnter() {
    if (wouldSaveMaze) changeMenu(SAVE_MAZE);
    else changeMenu(SELECTION);
}

void appendTostring(char * s1, char * s2) {

}

void saveEnter() {
    saveMaze(saveName, menuMaze);
    changeMenu(SELECTION);
}

void saveLetter() {
    char * input = getBufferValue();

    char * newSaveName = NULL;
    newSaveName = calloc(strlen(saveName) + strlen(input) + 1, sizeof(char));
    newSaveName[0] = '\0';
    strcat(newSaveName, saveName);
    strcat(newSaveName, input);

    saveName = newSaveName;
}

void saveBackspace() {
    if (strlen(saveName) > 0) {
        char * newSaveName = NULL;
        newSaveName = calloc(strlen(saveName) - 2, sizeof(char));
        memccpy(newSaveName, saveName, strlen(saveName) - 2, sizeof(char));

        saveName = newSaveName;
    }
}

/** end event listenners functions */

void changeMenu(menuType newMenu) {
    currentMenu = newMenu;
    // all the action used in our menu
    rawTerminal_action  arrowDown = {0},
                        arrowUp = {0},
                        arrowLeft = {0},
                        arrowRight = {0},
                        enterPressed = {0},
                        escPressed = {0},
                        letterPressed = {0},
                        backspacePressed = {0};
    // setup for the action (only void functions supported)
    arrowDown.type = VOID_FUNCTION;
    arrowUp.type = VOID_FUNCTION;
    arrowLeft.type = VOID_FUNCTION;
    arrowRight.type = VOID_FUNCTION;
    enterPressed.type = VOID_FUNCTION;
    escPressed.type = VOID_FUNCTION;
    letterPressed.type = VOID_FUNCTION;
    backspacePressed.type = VOID_FUNCTION;

    switch (newMenu)
    {
    case SELECTION:
        arrowDown.func.void_function = &selectionDown;
        arrowUp.func.void_function = &selectionUp;
        enterPressed.func.void_function = &enterSelection;

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

        *newMaze = generateMaze(mazeCreationWidth, mazeCreationHeight);
        break;
    
    case LOAD_MAZE:
        // opening save folder
        loadMazeNumberOfFile = 0;
        saveFiles = NULL;
        saveFileSkipIndex = NULL;
        skipIndexSize = 0;
        loadMazeCurrentFocus = 0;

        DIR * d = NULL;
        struct dirent *dir;
        d = opendir("./data/maze");

        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if (loadMazeNumberOfFile == 0) {
                    saveFiles = calloc(1, sizeof(struct dirent));
                } else {
                    saveFiles = realloc(saveFiles, sizeof(struct dirent) * (loadMazeNumberOfFile + 1));
                }
                if (dir->d_type != DT_REG) {
                    if (saveFileSkipIndex == NULL) {
                        saveFileSkipIndex = calloc(1, sizeof(int));
                    } else {
                        saveFileSkipIndex = realloc(saveFileSkipIndex, sizeof(int) * (skipIndexSize + 1));
                    }
                    saveFileSkipIndex[skipIndexSize] = loadMazeNumberOfFile;
                    skipIndexSize++;
                }
                saveFiles[loadMazeNumberOfFile] = *dir;
                loadMazeNumberOfFile++;
            }
            closedir(d);
        } // TODO handle error

        while (isInSkipIndex(loadMazeCurrentFocus) && loadMazeCurrentFocus < (loadMazeNumberOfFile + 1))
        {
            loadMazeCurrentFocus++;
        }

        arrowDown.func.void_function = &loadDown;
        arrowUp.func.void_function = &loadUp;
        enterPressed.func.void_function = &loadEnter;
        break;
    case PLAY:
        menuMazeHandler = initMazeMovement(&menuMaze);

        arrowDown.func.void_function = &playDown;
        arrowUp.func.void_function = &playUp;
        arrowLeft.func.void_function = &playLeft;
        arrowRight.func.void_function = &playRight;
        escPressed.func.void_function = &playEsc;
        break;
    case SAVE_MAZE_CHOICE:
        wouldSaveMaze = true;
        arrowLeft.func.void_function = &saveChoiceLeft;
        arrowRight.func.void_function = &saveChoiceRight;
        enterPressed.func.void_function = &saveChoiceEnter;
        break;
    case SAVE_MAZE_LOADING:
        break;
    case SAVE_MAZE:
        saveName = "";
        letterPressed.func.void_function = &saveLetter;
        backspacePressed.func.void_function = &saveBackspace;
        enterPressed.func.void_function = &saveEnter;
        break;
    default: break;
    }

    if (arrowDown.func.void_function != NULL) assignActionToKeyBoardInput(INPUT_ARROW_DOWN, &arrowDown);
    if (arrowUp.func.void_function != NULL) assignActionToKeyBoardInput(INPUT_ARROW_UP, &arrowUp);
    if (arrowRight.func.void_function != NULL) assignActionToKeyBoardInput(INPUT_ARROW_RIGHT, &arrowRight);
    if (arrowLeft.func.void_function != NULL) assignActionToKeyBoardInput(INPUT_ARROW_LEFT, &arrowLeft);
    if (enterPressed.func.void_function != NULL) assignActionToKeyBoardInput(INPUT_ENTER, &enterPressed);
    if (escPressed.func.void_function != NULL) assignActionToKeyBoardInput(INPUT_ESCAPE_CHAR, &escPressed);
    if (letterPressed.func.void_function != NULL) assignActionToKeyBoardInput(INPUT_LETTER, &letterPressed);
    if (backspacePressed.func.void_function != NULL) assignActionToKeyBoardInput(INPUT_BACKSPACE, &backspacePressed);
}

void display() {
    // if (errorMessage != NULL) printfColored(WHITE, RED, BOLD, "/!\\ %s /!\\ \n", errorMessage);
    switch (currentMenu)
    {
    case SELECTION:
        printfColored(BLACK, WHITE, UNDERLINE, "- SELECTION -\n");

        if (menuMaze.height != 0 && menuMaze.width != 0) displayMaze(menuMaze);

        if (selectionChoice == CREATE_MAZE) {
            printfColored(BLACK, WHITE, BOLD, "Create maze");
        } else {
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "Create maze");
        }
        printf("\n");
        if (selectionChoice == LOAD_MAZE) {
            printfColored(BLACK, WHITE, BOLD, "Load maze");
        } else {
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "Load maze");
        }
        printf("\n");
        if (selectionChoice == PLAY) {
            printfColored(BLACK, WHITE, BOLD, "Play");
        } else {
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "Play");
        }
        printf("\n");
        if (selectionChoice == EXIT) {
            printfColored(BLACK, WHITE, BOLD, "Exit");
        } else {
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "Exit");
        }
        printf("\n");
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
        printfColored(YELLOW, DEFAULT_COLOR, BOLD, "%d save files found (%d files in the folder)\n", loadMazeNumberOfFile - skipIndexSize, loadMazeNumberOfFile);
        if (loadMazeCurrentFocus != loadMazeNumberOfFile) {
            for (int i = 0; i < loadMazeNumberOfFile; i++)
            {
                if (saveFiles[i].d_type == DT_REG) {
                    if (loadMazeCurrentFocus == i) {
                        printfColored(WHITE, GREEN, BOLD, saveFiles[i].d_name);
                        printfColored(GREEN, DEFAULT_COLOR, BOLD, "[load this save]");
                    } else printfColored(YELLOW, DEFAULT_COLOR, UNDERLINE, saveFiles[i].d_name);
                    printf("\n");
                }
            }
            printfColored(RED, DEFAULT_COLOR, BOLD, "Back to menu\n");
        } else {
            for (int i = 0; i < loadMazeNumberOfFile; i++)
            {
                if (saveFiles[i].d_type == DT_REG) {
                    printfColored(YELLOW, DEFAULT_COLOR, UNDERLINE, saveFiles[i].d_name);
                    printf("\n");
                }
            }
            printfColored(WHITE, RED, BOLD, "Back to menu\n");
        }
        break;
    case PLAY:
        printfColored(BLACK, WHITE, UNDERLINE, "- PLAY -\n");
        displayMazeWithPlayer(menuMazeHandler);
        printfColored(RED, DEFAULT_COLOR, ITALIC, "press [esc] to return to selection menu");
        printf("");
        break;
    case SAVE_MAZE_CHOICE:
        printfColored(BLACK, WHITE, UNDERLINE, "- SAVE MAZE -\n");
        displayMaze(menuMaze);
        printfColored(WHITE, DEFAULT_COLOR, BOLD, "Would you like to save this maze ?");
        printf("\n");
        printfColored(!wouldSaveMaze ? WHITE : BLACK, !wouldSaveMaze ? DEFAULT_COLOR : WHITE, BOLD, "y"); // TODO change terner
        printf(" - ");
        printfColored(wouldSaveMaze ? WHITE : BLACK, wouldSaveMaze ? DEFAULT_COLOR : WHITE, BOLD, "n"); // TODO change terner
        printf("\n");
        break;
    case SAVE_MAZE_LOADING:
        printfColored(BLACK, WHITE, UNDERLINE, "- SAVE MAZE -\n");
        printfColored(YELLOW, DEFAULT_COLOR, BOLD, "Saving the maze ...");
        printf("\n");
        break;
    case SAVE_MAZE:
        printfColored(BLACK, WHITE, UNDERLINE, "- SAVE MAZE -\n");
        printfColored(WHITE, DEFAULT_COLOR, BOLD, "Your file name : ");
        printfColored(YELLOW, DEFAULT_COLOR, UNDERLINE, "%s", saveName);
        printf("\n");
        break;
    default: break;
    }
    printf("\n");
}

void launchMenu() {

    enableTerminalRawMode();
    makeCursorInvisible();
    // setup for the menu
    changeMenu(SELECTION);
    // action in the terminal
    while (currentMenu != EXIT)
    {
        // clean the termial for new display
        home();
        clearTerminal();
        // clearAllTerminal();

        display();

        char * input = HandleRawModeKeyboard();
        // if (errorMessage != NULL) {
        //     free(errorMessage);
        //     errorMessage = NULL;
        // }
    }
    makeCursorVisible();
    disableTerminalRawMode();
}