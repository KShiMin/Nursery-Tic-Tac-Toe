/* Define preprocessor statements */
#include "gui.h"        /* Include gui header file */

/* Declare global variables used across files */
char player = O_PLAYER;         /* Initialise player as O. O always starts first */
int gameMode = PVP;             /* Initialise game mode as PVP */
int gameState = STATE_MENU;     /* Initialise game state as main menu - GUI shows main menu first*/
char board[3][3] = {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};       /* Initialise empty board */
int num_wins = 0;               /* Initialise number of wins + draws for CPU */


/********************************************************
function: getBoundary
    returns the end point for the tic tac toe grid

return: int 
********************************************************/
int getBoundary()
{
    return SCREEN_WIDTH - GRID_OFFSET;
}

/********************************************************
function: getCellSize
    returns the length of each tic tac toe grid

return: int
********************************************************/
int getCellSize()
{
    return (SCREEN_WIDTH - GRID_OFFSET * 2) / 3;
}

/********************************************************
function: game_start
    function called in main for every frame refresh.
    handles all the drawing based on gameState
********************************************************/
void game_start()
{
    if (gameState == STATE_MENU)        /* Main menu state */
    {
        draw_menu();    /* call draw_menu function */
    }

    else
    {
        draw_grid();                                /* call draw_grid function */
        draw_markers();                             /* call draw_markers function */
        displayScoreBoard();                        /* call displayScoreBoard function */
        displayCurrentPlayer();                     /* call displayCurrentPlayer function */
        gameState = check_board_status(board);      /* call check_board_status function from game_logic.c. change gameState based on function output */

        if (gameState == STATE_WIN || gameState == STATE_DRAW)      /* Game over state */
        {
            scoreBoard();       /* call scoreBoard function */
            game_over();        /* call game_over function */
        }
        else                                                        /* Game ongoing */                                 
        {
            CheckMouseInput();      /* call CheckMouseInput function */

            /* call check_board_status function from game_logic.c. change gameState based on function output */
            if (gameState == STATE_PLAYING && gameMode != PVP && player == X_PLAYER) {
                if (gameMode == PVC) {              /* gameMode is PVC and CPU turn */
                    // Call minimax algorithm
                } else if (gameMode == PVML) {      /* gameMode is PVML and CPU turn*/
                    // Call ML algorithm
                }
            }
        }

    }


}

/********************************************************
function: game_over
    draws game over prompt. displays winner if there 
    is one, else displays tie. shows button to restart
    game
********************************************************/
void game_over()
{
    Rectangle display = {200, 350, 600, 300};       /* Initialise prompt window  */

    if (gameState == STATE_WIN)                     /* Game over due to win */
    {
        const char *text = (player == O_PLAYER) ? "Player X Wins!" : "Player O Wins!";      /* Declare message string based on winner */
        DrawRectangleRec(display, WHITE);       /* Draw prompt window */
        DrawText(text, 250, 450, 70, BLACK);    /* Draw message string */    
    }

    else if (gameState == STATE_DRAW)               /* Game over due to draw */
    {
        DrawRectangleRec(display, WHITE);       /* Draw prompt window */
        DrawText("It's a Draw!", 300, 450, 70, BLACK);      /* Draw prompt window */
    }
    restartButton();    /* Call restartButton function */

}

/********************************************************
function: draw_menu
    draws the main menu. Checks mouse input for button 
    pressed and changes gameMode and gameState    
********************************************************/
void draw_menu()
{
    DrawText("TIC TAC TOE", 150, 200, 100, WHITE);              /* Draw game title */
    DrawText("Choose a game mode:", 280, 350, 40, WHITE);       /* Draw game mode prompt */

    Rectangle buttons[3] = {
        {215, 450, 550, 70},
        {215, 600, 550, 70},
        {215, 750, 550, 70}
    };

    const char *labels[3] = {
        "Player vs Player",
        "Play with Computer 1",
        "Play with Computer 2"
    };
    
    const int textOffset[3] = {
        85,
        45,
        45
    };

    for (int i = 0; i < 3; i++) {
        DrawRectangleRec(buttons[i], WHITE);
        DrawText(labels[i], buttons[i].x + textOffset[i], buttons[i].y + 15, 45, BLACK);
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();

        for (int i = 0; i < 3; i++) {
            if (CheckCollisionPointRec(mousePos, buttons[i])) {
                gameMode = i;
                gameState = STATE_PLAYING;
                break;
            }
        }
    }

    /* PvP button */
    // Rectangle pvpButton = {215, 450, 550, 70};                  /* Initialise pvpButton */
    // DrawRectangleRec(pvpButton, WHITE);                         /* Draws pvpButton */
    // DrawText("Player vs Player", pvpButton.x + 85, pvpButton.y + 15, 45, BLACK);    /* Draws text on pvpButton */

    // /* CPU button */
    // Rectangle cpuButton = {215, 600, 550, 70};                  /* Initialise cpuButton */
    // DrawRectangleRec(cpuButton, WHITE);                         /* Draws cpuButton */
    // DrawText("Play with Computer 1", cpuButton.x + 45, cpuButton.y + 15, 45, BLACK);    /* Draws text on cpuButton */

    // /* CPU ML button*/
    // Rectangle mlButton = {215, 750, 550, 70};                  /* Initialise mlButton */
    // DrawRectangleRec(mlButton, WHITE);                         /* Draws mlButton */
    // DrawText("Play with Computer 2", mlButton.x + 45, mlButton.y + 15, 45, BLACK);      /* Draws text on mlButton */

    // if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))               /* Left mouse button pressed */
    // {
    //     Vector2 mousePos = GetMousePosition();              /* Gets mouse coordinates, store in mousePos */

    //     if (CheckCollisionPointRec(mousePos, pvpButton))    /* Mouse coordinates is within pvpButton */
    //     {
    //         gameMode = PVP;                 /* set gameMode to PVP */
    //         gameState = STATE_PLAYING;      /* set gameState to STATE_PLAYING */
    //     }

    //     else if (CheckCollisionPointRec(mousePos, cpuButton))       /* Mouse coordinates is within cpuButton */
    //     {
    //         gameMode = PVC;                 /* set gameMode to PVC */
    //         gameState = STATE_PLAYING;      /* set gameState to STATE_PLAYING */
    //     }

    //     else if (CheckCollisionPointRec(mousePos, mlButton))        /* Mouse coordinates is within mlButton */
    //     {
    //         gameMode = PVML;                /* set gameMode to PVML */
    //         gameState = STATE_PLAYING;      /* set gameState to STATE_PLAYING */
    //     }

    // }
}


/********************************************************
function: draw_grid
    draws the tic tac toe grid
********************************************************/
void draw_grid()
{
    int cellSize = getCellSize();       /* initiates cellSize to output of getCellSize function */

    /* Draw tic tac toe grid */
    for (int i = 1; i < 3; i++)         /* Iterates twice for each of the two vertical and horizontal lines  */
    {
        DrawLine(GRID_OFFSET + i * cellSize, GRID_OFFSET, GRID_OFFSET + i * cellSize, SCREEN_HEIGHT - GRID_OFFSET, WHITE);      /* Draws the i-th vertical line  */
        DrawLine(GRID_OFFSET, (GRID_OFFSET + i * cellSize), SCREEN_WIDTH - GRID_OFFSET, GRID_OFFSET + i * cellSize, WHITE);     /* Draws the i-th horizontal line */
    }

}

/********************************************************
function: CheckMouseInput
    gets move input from user. Updates tic tac toe
    board if mouse input is within the game board
********************************************************/
void CheckMouseInput()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))        /* Left mouse button pressed */
    {
        Vector2 mousePos = GetMousePosition();      /* Gets mouse coordinates, store in mousePos */
        int boundary = getBoundary();               /* initiates boundary to output of getBoundary function */

        if (mousePos.x > GRID_OFFSET && mousePos.x < boundary && mousePos.y > GRID_OFFSET && mousePos.y < boundary)     /* Checks if mouse input is in one of the grids */
        {
            int cellSize = getCellSize();                           /* calls getCellSize function */
            int row = (mousePos.y - GRID_OFFSET) / cellSize;        /* gets the row of the tic tac toe cell selected */
            int col = (mousePos.x - GRID_OFFSET) / cellSize;        /* gets the column of the tic tac toe cell selected */
            while (board[row][col] == EMPTY)                          /* checks board if selected tic tac toe cell is */
            {
                update_board(board, row, col, player);      /* calls update_board function from game_logic.c */
                player = (player == X_PLAYER) ? O_PLAYER : X_PLAYER;    /* changes player to the next player */
            }

            print_board(board);     /* calls print_board function from game_logic.c */

            

        }
    }
}

/********************************************************
function: draw_makers
    draws the markers if contents of board is not empty
********************************************************/
void draw_markers()
{
    for (int i = 0; i < 3; i++)         /* Iterates through rows of board */
    {
        for (int j = 0; j < 3; j++)     /* Iterates through columns of board */
        {
            if (board[i][j] != EMPTY)     /* Cell is not empty */
            {
                (board[i][j] == 'O') ? draw_o(i, j) : draw_x(i, j);
                // if (board[i][j] == 'O')     /* Contents of cell is O */
                // {
                //     draw_o(i, j);      /* Draw O in cell */
                // }

                // else                        /* Contents of cell is X */
                // {
                //     draw_x(i, j);       /* Draw X in cell */
                // }
            }
        }
    }
}

/********************************************************
function: draw_o
    draws the O marker based on row and col

inputs: row - integer row in which O is to be drawn
        col - integer column in which O is to be drawn
********************************************************/
void draw_o(int row, int col)
{
    int cellSize = getCellSize();                               /* initiates cellSize to output of getCellSize function */
    int y = (cellSize * row + cellSize / 2) + GRID_OFFSET;      /* initiates y, the centre of grid */
    int x = (cellSize * col + cellSize / 2) + GRID_OFFSET;      /* initiates x, the centre of grid */

    /* Draw O */
    DrawCircle(x, y, GRID_OFFSET / 2, WHITE);
    DrawCircle(x, y, GRID_OFFSET / 2 - 1, DARKBLUE);
}

/********************************************************
function: draw_x
    draws the X marker based on row and col

inputs: row - integer row in which X is to be drawn
        col - integer column in which X is to be drawn
********************************************************/
void draw_x(int row, int col)
{
    int cellSize = getCellSize();                           /* initiates cellSize to output of getCellSize function */
    
    /* Initialise corners of X markers */
    int y1 = cellSize * row + GRID_OFFSET + 50;
    int y2 = cellSize * (row + 1) + GRID_OFFSET - 50;
    int x1 = cellSize * col + GRID_OFFSET + 50;
    int x2 = cellSize * (col + 1) + GRID_OFFSET - 50;

    /* Draw X */
    DrawLine(x1, y1, x2, y2, WHITE);
    DrawLine(x1, y2, x2, y1, WHITE);
}

/********************************************************
function: restartButton
    displays a restart button in game over screen
********************************************************/
void restartButton()
{
    Rectangle restartButton = {350, 550, 300, 50};          /* Initialise restartButton */
    
    DrawRectangleRec(restartButton, LIGHTGRAY);             /* Draw restartButton */
    DrawText("Restart Game", 400, 560, 30, DARKGRAY);       /* Draw text on restartButton */
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))            /* Left mouse button pressed */
    {
        Vector2 mousePos = GetMousePosition();      /* Gets mouse coordinates, store in mousePos */

        if (CheckCollisionPointRec(mousePos, restartButton))    /* restartButton is clicked */
        {
            restartBoard(board);            /* call restartBoard function from game_logic.c */
            gameState = STATE_PLAYING;      /* set gameState to STATE_PLAYING */
        }
    }
}

/********************************************************
function: scoreBoard
    draws scoreboard text showing number of wins for
    player O and player X
********************************************************/
void displayScoreBoard() 
{
    char scoreMessage[50];
    sprintf(scoreMessage, "Score - Player O: %d | Player X: %d", player1Score, player2Score);
    DrawText(scoreMessage, 10, 10, 30, BLACK);
}

/********************************************************
function: displayCurrentPlayer
    draws current player text
********************************************************/
void displayCurrentPlayer()
{
    char turnMessage[50];
    sprintf(turnMessage, "Player %c's Turn", player);
    DrawText(turnMessage, 375, 75, 30, RAYWHITE);
}
