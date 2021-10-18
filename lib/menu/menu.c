#include "menu.h"

// TODO => error in save (while saving)
// TODO => notification in app (error or infos)
// TODO => conditionnal display (Victor)
// TODO create file score at start

menuType currentMenu = SELECTION;       // the menu currently in use
maze menuMaze = {0};                    // the maze in use in the function
// selection
leaderBoardScore * scores = NULL;       // leader board scores
menuType selectionChoice = CREATE_MAZE; // the current menu item selected in the SELECTION menu
// create maze
int mazeCreationWidth = 5;              // the width of the maze which will be created
int mazeCreationHeight = 5;             // the height of the meaz which will be created
int mazeCreationCurrentFocus = 0;       // the current menu item selected
maze * newMaze = NULL;                  // the maze created in the creation menu
char * nameGenerated = NULL;            // the name of the maze created
bool isCreatingHardcore = false;        // boolean influencing the maze creation
// load maze
int loadMazeNumberOfFile = 0;           // the number of save file found
struct dirent * saveFiles = NULL;       // the array of save file loaded
int * saveFileSkipIndex = NULL;         // the index of the file which does not correspond to a save
int skipIndexSize = 0;                  // the size of the array of file to skip
int loadMazeCurrentFocus = 0;           // the current menu item selected
// play
mazeHandler menuMazeHandler = {0};      // the maze handler used to play
// save maze
bool wouldSaveMaze = true;              // bool used to ask if the user want to save the current maze
// best score
char * bestScoreName = NULL;

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
        *newMaze = generateMaze(mazeCreationWidth, mazeCreationHeight, nameGenerated, isCreatingHardcore ? HARDCORE_MODE : NORMAL_MODE );
        break;
    case 1:
        if (mazeCreationHeight < 6) { break; } // TODO error
        mazeCreationHeight-=2;
        *newMaze = generateMaze(mazeCreationWidth, mazeCreationHeight, nameGenerated, isCreatingHardcore ? HARDCORE_MODE : NORMAL_MODE );
        break;
    case 2:
        isCreatingHardcore = !isCreatingHardcore;
        *newMaze = generateMaze(mazeCreationWidth, mazeCreationHeight, nameGenerated, isCreatingHardcore ? HARDCORE_MODE : NORMAL_MODE );
    default: break;
    }
}

void createRight() {
    switch (mazeCreationCurrentFocus)
    {
    case 0:
        mazeCreationWidth+=2;
        *newMaze = generateMaze(mazeCreationWidth, mazeCreationHeight, nameGenerated, isCreatingHardcore ? HARDCORE_MODE : NORMAL_MODE );
        break;
    case 1:
        mazeCreationHeight+=2;
        *newMaze = generateMaze(mazeCreationWidth, mazeCreationHeight, nameGenerated, isCreatingHardcore ? HARDCORE_MODE : NORMAL_MODE );
        break;
    case 2:
        isCreatingHardcore = !isCreatingHardcore;
        *newMaze = generateMaze(mazeCreationWidth, mazeCreationHeight, nameGenerated, isCreatingHardcore ? HARDCORE_MODE : NORMAL_MODE );
    default: break;
    }}

void createEnter() {
    switch (mazeCreationCurrentFocus)
    {
    case 3:
        *newMaze = generateMaze(mazeCreationWidth, mazeCreationHeight, nameGenerated, isCreatingHardcore ? HARDCORE_MODE : NORMAL_MODE );
        break;
    case 4:
        changeMenu(SELECTION);
        menuMaze = *newMaze;
        newMaze = NULL;
    case 5:
        changeMenu(SELECTION);
        break;
    default: break;
    }
}

void createLetter() {
    char * input = getBufferValue();

    char * newCreateName = NULL;
    newCreateName = calloc(strlen(nameGenerated) + strlen(input) + 1, sizeof(char));
    newCreateName[0] = '\0';
    strcat(newCreateName, nameGenerated);
    strcat(newCreateName, input);

    nameGenerated = newCreateName;

    newMaze->name = nameGenerated;
}

void createBackspace() {
    if (strlen(nameGenerated) > 0) {
        char * newCreateName = NULL;
        newCreateName = calloc(strlen(nameGenerated), sizeof(char));
        strncpy(newCreateName, nameGenerated, strlen(nameGenerated) - 1);

        nameGenerated = newCreateName;

        newMaze->name = nameGenerated;
    }
}

/** load maze event listenner */
// TODO - filename [suppr] del file
// TODO - dnt print all files but portion with an indicator showing there are more
// TODO - print preview of the save

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

int scoreIsHigherThanLeaderBoard(int currentScore) {
    for (int i = 0; i < MAX_NUMBER_OF_SCORE; i++)
    {
        if (currentScore > (scores[i].score)) {
            return i;
        }
    }
    return -1;
}

void checkEndGame() {
    if (menuMazeHandler.maze->elements[menuMazeHandler.mazePlayer.position.x][menuMazeHandler.mazePlayer.position.y] == MAZE_EXIT) {
        int indexInSCore = scoreIsHigherThanLeaderBoard(menuMazeHandler.score);
        if (indexInSCore != -1) {
            changeMenu(BEST_SCORE);
        } else changeMenu(SELECTION);
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

void playEnter() {
    saveMaze(*menuMazeHandler.maze);
}

/** save event listenner */
void saveChoiceLeft() {
    wouldSaveMaze = !wouldSaveMaze;
}

void saveChoiceRight() {
    wouldSaveMaze = !wouldSaveMaze;
}

/**
 * return the index of the score which is lower than current esle -1
*/

void saveChoiceEnter() {
    if (wouldSaveMaze) {
        saveMaze(menuMaze);
    }
    changeMenu(SELECTION);
}

/** best score event listenner */

void bestScoreLetter() {
    if (strlen(bestScoreName) < 20) {
        char * input = getBufferValue();

        char * newNameBestScore = NULL;
        newNameBestScore = calloc(strlen(bestScoreName) + strlen(input) + 1, sizeof(char));
        newNameBestScore[0] = '\0';
        strcat(newNameBestScore, bestScoreName);
        strcat(newNameBestScore, input);

        bestScoreName = newNameBestScore;
    }
}

void bestScoreBackspace() {
    if (strlen(bestScoreName) > 0) {
        char * newBestScore = NULL;
        newBestScore = calloc(strlen(bestScoreName), sizeof(char));
        strncpy(newBestScore, bestScoreName, strlen(bestScoreName) - 1);

        bestScoreName = newBestScore;
    }
}

void insertValue(int index, int score, char * name) {
    if (index + 1 < MAX_NUMBER_OF_SCORE) insertValue(index+1, scores[index].score, scores[index].name);

    scores[index].score = score;

    char * newName = calloc(20, sizeof(char));
    strcpy(newName, name);
    scores[index].name = newName;
}

void saveBestScore() {
    int index = scoreIsHigherThanLeaderBoard(menuMazeHandler.score);
    insertValue(index, menuMazeHandler.score, bestScoreName);

    FILE * f = fopen(LEADERBOARD_FILE, "w");
    for (int i = 0; i < MAX_NUMBER_OF_SCORE; i++)
    {
        fwrite(&(scores[i].score), sizeof(int), 1, f);
        fwrite(scores[i].name, sizeof(char), 20, f);
    }
    fclose(f);
}

void bestScoreEnter() {
    changeMenu(SELECTION);
    saveBestScore();
}

/**
 * end event listenners functions
 */

void initRawTerminalActionForDefaultAction(rawTerminal_action * action) {
    action->func.void_function = DUMMY_FUNCTION;
    action->type = VOID_FUNCTION;
}

// TODO error fread
void loadLeaderBoard() {
        FILE * leaderboardFile = fopen(LEADERBOARD_FILE, "r");
        scores = calloc(10, sizeof(leaderBoardScore));
        for (int i = 0; i < MAX_NUMBER_OF_SCORE; i++)
        {
            leaderBoardScore temp = {0};

            fread(&(temp.score), sizeof(int), 1, leaderboardFile);

            temp.name = calloc(20, sizeof(char));
            fread(temp.name, sizeof(char), 20, leaderboardFile);

            scores[i] = temp;
        }
        fclose(leaderboardFile);
}

void selectionInit(rawTerminal_action *down, rawTerminal_action *up, rawTerminal_action *enter) {
        down->func.void_function = &selectionDown;
        up->func.void_function = &selectionUp;
        enter->func.void_function = &enterSelection;

        selectionChoice = CREATE_MAZE;

        loadLeaderBoard();
}

void createMazeInit(rawTerminal_action *down, rawTerminal_action *up, rawTerminal_action *left, rawTerminal_action *right, rawTerminal_action *enter, rawTerminal_action *letter, rawTerminal_action *backspace) {
        newMaze = calloc(1, sizeof(maze));
        mazeCreationCurrentFocus = 0;
        nameGenerated = "newMaze";
        isCreatingHardcore = false;

        down->func.void_function = &createDown;
        up->func.void_function = &createUp;
        left->func.void_function = &createLeft;
        right->func.void_function = &createRight;
        enter->func.void_function = &createEnter;
        letter->func.void_function = &createLetter;
        backspace->func.void_function = &createBackspace;

        *newMaze = generateMaze(mazeCreationWidth, mazeCreationHeight, nameGenerated, NORMAL_MODE);
}

void loadMazeInit(rawTerminal_action *down, rawTerminal_action *up, rawTerminal_action *enter) {
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

    down->func.void_function = &loadDown;
    up->func.void_function = &loadUp;
    enter->func.void_function = &loadEnter;
}

void playInit(rawTerminal_action *down, rawTerminal_action *up, rawTerminal_action *left, rawTerminal_action *right, rawTerminal_action *z, rawTerminal_action *q, rawTerminal_action *s, rawTerminal_action *d, rawTerminal_action *esc, rawTerminal_action * enter) {
        // reseting the data in the maze
        // cleanup reset code
        for (int i = 0; i < menuMaze.numberOfEntity; i++)
        {
            menuMaze.entities[i].isAlive = true;
        }
        menuMaze.isUnlocked = false;
        
        menuMazeHandler = initMazeMovement(&menuMaze);

        down->func.void_function = &playDown;
        up->func.void_function = &playUp;
        left->func.void_function = &playLeft;
        right->func.void_function = &playRight;

        z->func.void_function = &playUp;
        q->func.void_function = &playLeft;
        s->func.void_function = &playDown;
        d->func.void_function = &playRight;

        esc->func.void_function = &playEsc;
        enter->func.void_function = &playEnter;
}

void saveMazeInit(rawTerminal_action *left, rawTerminal_action *right, rawTerminal_action *enter) {
        wouldSaveMaze = true;

        left->func.void_function = &saveChoiceLeft;
        right->func.void_function = &saveChoiceRight;
        enter->func.void_function = &saveChoiceEnter;
}

void initAllRawTerminalActions(rawTerminal_action *up, rawTerminal_action *down, rawTerminal_action *right, rawTerminal_action *left, rawTerminal_action *z, rawTerminal_action *q, rawTerminal_action *s, rawTerminal_action *d, rawTerminal_action *enter, rawTerminal_action *esc, rawTerminal_action *backspace, rawTerminal_action *letter) {
    initRawTerminalActionForDefaultAction(up);
    initRawTerminalActionForDefaultAction(down);
    initRawTerminalActionForDefaultAction(right);
    initRawTerminalActionForDefaultAction(left);
    initRawTerminalActionForDefaultAction(z);
    initRawTerminalActionForDefaultAction(q);
    initRawTerminalActionForDefaultAction(s);
    initRawTerminalActionForDefaultAction(d);
    initRawTerminalActionForDefaultAction(enter);
    initRawTerminalActionForDefaultAction(backspace);
    initRawTerminalActionForDefaultAction(esc);
    initRawTerminalActionForDefaultAction(letter);
}

void assignAllRawTerminalActions(rawTerminal_action *up, rawTerminal_action *down, rawTerminal_action *right, rawTerminal_action *left, rawTerminal_action *z, rawTerminal_action *q, rawTerminal_action *s, rawTerminal_action *d, rawTerminal_action *enter, rawTerminal_action *esc, rawTerminal_action *backspace, rawTerminal_action *letter) {
    assignActionToKeyBoardInput(INPUT_ARROW_UP, up);
    assignActionToKeyBoardInput(INPUT_ARROW_DOWN, down);
    assignActionToKeyBoardInput(INPUT_ARROW_RIGHT, right);
    assignActionToKeyBoardInput(INPUT_ARROW_LEFT, left);
    assignActionToKeyBoardInput(INPUT_z, z);
    assignActionToKeyBoardInput(INPUT_q, q);
    assignActionToKeyBoardInput(INPUT_s, s);
    assignActionToKeyBoardInput(INPUT_d, d);
    assignActionToKeyBoardInput(INPUT_ENTER, enter);
    assignActionToKeyBoardInput(INPUT_ESCAPE_CHAR, esc);
    assignActionToKeyBoardInput(INPUT_BACKSPACE, backspace);
    assignActionToKeyBoardInput(INPUT_LETTER, letter);
}

void changeMenu(menuType newMenu) {
    currentMenu = newMenu;

    rawTerminal_action arrowUp = {0};
    rawTerminal_action arrowDown = {0};
    rawTerminal_action arrowRight = {0};
    rawTerminal_action arrowLeft = {0};
    rawTerminal_action zPressed = {0};
    rawTerminal_action qPressed = {0};
    rawTerminal_action sPressed = {0};
    rawTerminal_action dPressed = {0};
    rawTerminal_action enterPressed = {0};
    rawTerminal_action escPressed = {0};
    rawTerminal_action backspacePressed = {0};
    rawTerminal_action letterPressed = {0};

    initAllRawTerminalActions(
        &arrowUp,
        &arrowDown,
        &arrowRight,
        &arrowLeft,
        &zPressed,
        &qPressed,
        &sPressed,
        &dPressed,
        &enterPressed,
        &escPressed,
        &backspacePressed,
        &letterPressed
    );

    switch (newMenu)
    {
    case SELECTION:
        selectionInit(&arrowDown, &arrowUp, &enterPressed);
        break;
    case CREATE_MAZE:
        createMazeInit(&arrowDown, &arrowUp, &arrowLeft, &arrowRight, &enterPressed, &letterPressed, &backspacePressed);
        break;
    case LOAD_MAZE:
        loadMazeInit(&arrowDown, &arrowUp, &enterPressed);
        break;
    case PLAY:
        playInit(&arrowDown, &arrowUp, &arrowLeft, &arrowRight, &zPressed, &qPressed, &sPressed, &dPressed, &escPressed, &enterPressed);
        break;
    case SAVE_MAZE_CHOICE:
        saveMazeInit(&arrowLeft, &arrowRight, &enterPressed);
        break;
    case BEST_SCORE:
        bestScoreName = "Yay I am the best !";
        letterPressed.func.void_function = &bestScoreLetter;
        backspacePressed.func.void_function = &bestScoreBackspace;
        enterPressed.func.void_function = &bestScoreEnter;
        break;
    default: break;
    }
    
    assignAllRawTerminalActions(
        &arrowUp,
        &arrowDown,
        &arrowRight,
        &arrowLeft,
        &zPressed,
        &qPressed,
        &sPressed,
        &dPressed,
        &enterPressed,
        &escPressed,
        &backspacePressed,
        &letterPressed
    );
}

void displayLeaderboard() {
    printfColored(BLACK, WHITE, UNDERLINE, "- LEADERBOARD -\n");
    printfColored(RED, DEFAULT_COLOR, BOLD, "%10d\t%s", scores[0].score, scores[0].name);
    printf("\n");
    printfColored(YELLOW, DEFAULT_COLOR, BOLD, "%10d\t%s", scores[1].score, scores[1].name);
    printf("\n");
    for (int i = 2; i < MAX_NUMBER_OF_SCORE; i++)
    {
        printfColored(WHITE, DEFAULT_COLOR, BOLD, "%10d\t%s", scores[i].score, scores[i].name);
        printf("\n");
    }
}

void displaySelectionMenuElement(char * content, menuType type) {
    if (selectionChoice == type) {
        printfColored(BLACK, WHITE, BOLD, "%s", content);
    } else {
        printfColored(WHITE, DEFAULT_COLOR, BOLD, "%s", content);
    }
}

void displaySelectionMenu() {
    displaySelectionMenuElement("Create maze", CREATE_MAZE);
    printf("\n");
    displaySelectionMenuElement("Load maze", LOAD_MAZE);
    printf("\n");
    displaySelectionMenuElement("Play", PLAY);
    printf("\n");
    displaySelectionMenuElement("Exit", EXIT);
    printf("\n");
}

void displaySelection() {
        printfColored(BLACK, WHITE, UNDERLINE, "- SELECTION -\n");
        printf("\n");
        displayLeaderboard();
        printf("\n");
        displayMaze(menuMaze);
        printf("\n");
        displaySelectionMenu();
}

// TODO cleanup display
void displayMazeCreation() {
        printfColored(BLACK, WHITE, UNDERLINE, "- CREATE MAZE -");
        printf("\n\n");
        printfColored(YELLOW, DEFAULT, UNDERLINE, "[INFO] type any char to change the name (you can erase with backspace)");
        printf("\n\n");

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
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "hardcore : ");
            if (isCreatingHardcore) printfColored(RED, DEFAULT_COLOR, BOLD, "y");
            else printfColored(GREEN, DEFAULT_COLOR, BOLD, "n");
            printf("\n");
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
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "hardcore : ");
            if (isCreatingHardcore) printfColored(RED, DEFAULT_COLOR, BOLD, "y");
            else printfColored(GREEN, DEFAULT_COLOR, BOLD, "n");
            printf("\n");
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "generate\n");
            printfColored(GREEN, DEFAULT_COLOR, BOLD, "load this maze\n");
            printfColored(RED, DEFAULT_COLOR, BOLD, "Back to menu\n");
            break; 
        case 2:
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "width :\t\t< %d >\n", mazeCreationWidth);
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "height:\t\t< %d >\n", mazeCreationHeight);
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "hardcore : ");
            if (isCreatingHardcore) printfColored(RED, WHITE, BOLD, "y");
            else printfColored(GREEN, WHITE, BOLD, "n");
            printf("\n");
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "generate\n");
            printfColored(GREEN, DEFAULT_COLOR, BOLD, "load this maze\n");
            printfColored(RED, DEFAULT_COLOR, BOLD, "Back to menu\n");
            break;
        case 3:
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "width :\t\t< %d >\n", mazeCreationWidth);
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "height:\t\t< %d >\n", mazeCreationHeight);
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "hardcore : ");
            if (isCreatingHardcore) printfColored(RED, DEFAULT_COLOR, BOLD, "y");
            else printfColored(GREEN, DEFAULT_COLOR, BOLD, "n");
            printf("\n");
            printfColored(BLACK, WHITE, BOLD, "generate\n");
            printfColored(GREEN, DEFAULT_COLOR, BOLD, "load this maze\n");
            printfColored(RED, DEFAULT_COLOR, BOLD, "Back to menu\n");
            break;
        case 4:
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "width :\t\t< %d >\n", mazeCreationWidth);
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "height:\t\t< %d >\n", mazeCreationHeight);
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "hardcore : ");
            if (isCreatingHardcore) printfColored(RED, DEFAULT_COLOR, BOLD, "y");
            else printfColored(GREEN, DEFAULT_COLOR, BOLD, "n");
            printf("\n");
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "generate\n");
            printfColored(WHITE, GREEN, BOLD, "load this maze\n");
            printfColored(RED, DEFAULT_COLOR, BOLD, "Back to menu\n");
            break;
        case 5:
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "width :\t\t< %d >\n", mazeCreationWidth);
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "height:\t\t< %d >\n", mazeCreationHeight);
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "hardcore : ");
            if (isCreatingHardcore) printfColored(RED, DEFAULT_COLOR, BOLD, "y");
            else printfColored(GREEN, DEFAULT_COLOR, BOLD, "n");
            printf("\n");
            printfColored(WHITE, DEFAULT_COLOR, BOLD, "generate\n");
            printfColored(GREEN, DEFAULT_COLOR, BOLD, "load this maze\n");
            printfColored(WHITE, RED, BOLD, "Back to menu\n");
            break;
        default: break;
        }
}

void displayloadMaze() {
    printfColored(BLACK, WHITE, UNDERLINE, "- LOAD MAZE -");
    printf("\n");

    printfColored(YELLOW, DEFAULT_COLOR, BOLD, "%d save files found (%d files in the folder)", loadMazeNumberOfFile - skipIndexSize, loadMazeNumberOfFile);
    printf("\n");

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
}

void displayPlay() {
    printfColored(BLACK, WHITE, UNDERLINE, "- PLAY -");
    printf("\n");

    displayMazeWithPlayer(menuMazeHandler);

    printfColored(GREEN, DEFAULT_COLOR, ITALIC, "press [enter] to return to save the maze");
    printf("\n");
    printfColored(RED, DEFAULT_COLOR, ITALIC, "press [esc] to return to selection menu");

    printf(" "); // reset terminal buffer to correct display errors (space to avoid warnings)
}

void displaySaveMazeChoice() {
    printfColored(BLACK, WHITE, UNDERLINE, "- SAVE MAZE -");
    printf("\n");

    displayMaze(menuMaze);
    printfColored(WHITE, DEFAULT_COLOR, BOLD, "Would you like to save this maze ?");
    printf("\n");

    if (wouldSaveMaze) {
        printfColored(GREEN, WHITE, BOLD, "y");
        printf(" - ");
        printfColored(DEFAULT_COLOR, DEFAULT_COLOR, BOLD, "n");
    } else {
        printfColored(DEFAULT_COLOR, DEFAULT_COLOR, BOLD, "y");
        printf(" - ");
        printfColored(RED, WHITE, BOLD, "n");
    }

    printf("\n");
}

void display() {
    switch (currentMenu)
    {
    case SELECTION:
        displaySelection();
        break;
    case CREATE_MAZE:
        displayMazeCreation();
        break;
    case LOAD_MAZE:
        displayloadMaze();
        break;
    case PLAY:
        displayPlay();
        break;
    case SAVE_MAZE_CHOICE:
        displaySaveMazeChoice();
        break;
    case BEST_SCORE:
        printfColored(WHITE, YELLOW, UNDERLINE, "You have set a new record !!!");
        printf("\n");
        printf("\n");
        printfColored(WHITE, DEFAULT_COLOR, DEFAULT_STYLE, "Please enter your name : ");
        printfColored(GREEN, DEFAULT_COLOR, UNDERLINE, "%s", bestScoreName);
        printf("\n");
        break;
    default: break;
    }
    printf("\n");
}

void launchMenu() {
    enableTerminalRawMode();
    makeCursorInvisible();

    if( !access( LEADERBOARD_FILE, F_OK ) == 0 ) {
        FILE * f = fopen(LEADERBOARD_FILE, "w");

        int writeReturn = 0;
        for (int i = 0; i < MAX_NUMBER_OF_SCORE; i++)
        {
            char * str = calloc(20, sizeof(char));
            int score = 0;
            writeReturn = fwrite(str, sizeof(char), 20, f);
            writeReturn = fwrite(&score, sizeof(int), 1, f);
        }

        fclose(f);
    }

    // initial state of the app
    menuMaze = generateMaze(10, 10, "startingMaze", NORMAL_MODE);
    changeMenu(SELECTION);

    // action in the terminal
    while (currentMenu != EXIT)
    {
        // clean the termial for new display
        home();
        clearTerminal();

        display();

        char * input = HandleRawModeKeyboard();
    }

    makeCursorVisible();
    disableTerminalRawMode();
}