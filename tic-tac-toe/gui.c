#include "raylib.h"
#include "gamelogic.h"
#include <stdio.h>
#include "gui.h"


char player = O_PLAYER;
int gameMode = PVP;
int gameState = STATE_MENU;
char board[3][3] = {'-', '-', '-', '-', '-', '-', '-', '-', '-'};
int difficulty = PERFECT;
int num_wins = 0;

/********************************************************
function: getBoundary
********************************************************/
int getBoundary()
{
    return SCREEN_WIDTH - GRID_OFFSET;
}

/********************************************************
function: getCellSize
********************************************************/
int getCellSize()
{
    return (SCREEN_WIDTH - GRID_OFFSET * 2) / 3;
}

/********************************************************
function: game_start
********************************************************/
void game_start()
{
    if (gameState == STATE_MENU)
    {
        draw_menu();
    }

    else if (gameState == STATE_PLAYING && gameEnded == 0)
    {
        draw_grid();
        draw_markers();
        displayScoreBoard();
        displayCurrentPlayer();
        gameState = check_board_status(board);

        if (gameState == STATE_WIN || gameState == STATE_DRAW)
        {
            gameEnded = 1;
            scoreBoard();
            game_over();
        }
        else
        {
            CheckMouseInput();
        }

        if (gameState == STATE_PLAYING)
            {
                if (gameMode == PVC && player == X_PLAYER)
                {
                    /* Call minimax algorithm! */
                }

            }
    }

    else if (gameEnded == 1)
    {
        draw_grid();
        draw_markers();
        displayScoreBoard();
        game_over();
    }

}

/********************************************************
function: game_over
********************************************************/
void game_over()
{
    Rectangle display = {200, 350, 600, 300};

    if (gameState == STATE_WIN)
    {
        const char *text = (player == O_PLAYER) ? "Player X Wins!" : "Player O Wins!";
        DrawRectangleRec(display, WHITE);
        DrawText(text, 250, 450, 70, BLACK);
        restartButton();
    }

    else if (gameState == STATE_DRAW)
    {
        DrawRectangleRec(display, WHITE);
        DrawText("It's a Draw!", 300, 450, 70, BLACK);
        restartButton();
    }

}

/********************************************************
function: draw_menu
********************************************************/
void draw_menu()
{
    DrawText("TIC TAC TOE", 150, 200, 100, WHITE);
    DrawText("Choose a game mode:", 280, 350, 40, WHITE);

    /* PvP button */
    Rectangle pvpButton = {215, 450, 550, 70};
    DrawRectangleRec(pvpButton, WHITE);
    DrawText("Player vs Player", pvpButton.x + 85, pvpButton.y + 15, 45, BLACK);

    /* CPU button */
    Rectangle cpuButton = {215, 600, 550, 70};
    DrawRectangleRec(cpuButton, WHITE);
    DrawText("Play with Computer 1", cpuButton.x + 45, cpuButton.y + 15, 45, BLACK);

    /* CPU ML button*/
    Rectangle mlButton = {215, 750, 550, 70};
    DrawRectangleRec(mlButton, WHITE);
    DrawText("Play with Computer 2", mlButton.x + 45, mlButton.y + 15, 45, BLACK);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mousePos = GetMousePosition();

        if (CheckCollisionPointRec(mousePos, pvpButton))
        {
            gameMode = PVP;
            gameState = STATE_PLAYING;
        }

        else if (CheckCollisionPointRec(mousePos, cpuButton))
        {
            gameMode = PVC;
            gameState = STATE_PLAYING;
        }

        else if (CheckCollisionPointRec(mousePos, mlButton))
        {
            gameMode = PVML;
            gameState = STATE_PLAYING;
        }

    }
}


/********************************************************
function: draw_grid
********************************************************/
void draw_grid()
{
    int cellSize = getCellSize();

    for (int i = 1; i < 3; i++)
    {
        DrawLine(GRID_OFFSET + i * cellSize, GRID_OFFSET, GRID_OFFSET + i * cellSize, SCREEN_HEIGHT - GRID_OFFSET, WHITE);
        DrawLine(GRID_OFFSET, (GRID_OFFSET + i * cellSize), SCREEN_WIDTH - GRID_OFFSET, GRID_OFFSET + i * cellSize, WHITE);
    }

}

/********************************************************
function: CheckMouseInput
********************************************************/
void CheckMouseInput()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mousePos = GetMousePosition();
        int boundary = getBoundary();

        if (mousePos.x > GRID_OFFSET && mousePos.x < boundary && mousePos.y > GRID_OFFSET && mousePos.y < boundary)
        {
            int cellSize = getCellSize();
            int row = (mousePos.y - GRID_OFFSET) / cellSize;
            int col = (mousePos.x - GRID_OFFSET) / cellSize;
            while (board[row][col] == '-')
            {
                update_board(board, row, col, player); player = (player == X_PLAYER) ? O_PLAYER : X_PLAYER;
            }

            print_board(board);

            

        }
    }
}

/********************************************************
function: draw_makers
********************************************************/
void draw_markers()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] != '-')
            {
                if (board[i][j] == 'O')
                {
                    draw_o(i, j);
                }

                else
                {
                    draw_x(i, j);
                }
            }
        }
    }
}

/********************************************************
function: draw_o
********************************************************/
void draw_o(int row, int col)
{
    int cellSize = getCellSize();
    int y = (cellSize * row + cellSize / 2) + GRID_OFFSET;
    int x = (cellSize * col + cellSize / 2) + GRID_OFFSET;
    DrawCircle(x, y, GRID_OFFSET/2, WHITE);
    DrawCircle(x, y, GRID_OFFSET/2 - 1, WHITE);
}

/********************************************************
function: draw_x
********************************************************/
void draw_x(int row, int col)
{
    int cellSize = getCellSize();
    int y1 = cellSize * row + GRID_OFFSET + 50;
    int y2 = cellSize * (row + 1) + GRID_OFFSET - 50;
    int x1 = cellSize * col + GRID_OFFSET + 50;
    int x2 = cellSize * (col + 1) + GRID_OFFSET - 50;

    DrawLine(x1, y1, x2, y2, WHITE);
    DrawLine(x1, y2, x2, y1, WHITE);
}

/********************************************************
function: restartButton
********************************************************/
void restartButton()
{
    Rectangle restartButton = {350, 550, 300, 50};
    
    DrawRectangleRec(restartButton, LIGHTGRAY);
    DrawText("Restart Game", 400, 560, 30, DARKGRAY);
    
    Vector2 mousePoint = GetMousePosition();
    if (CheckCollisionPointRec(mousePoint, restartButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        restartBoard(board);
        gameState = STATE_PLAYING; /*To indicate game is ongoing*/
    }
}

/********************************************************
function: scoreBoard
********************************************************/
void displayScoreBoard() 
{
    char scoreMessage[50];
    sprintf(scoreMessage, "Score - Player O: %d | Player X: %d", player1Score, player2Score);
    DrawText(scoreMessage, 10, 10, 30, BLACK);
}

/********************************************************
function: displayCurrentPlayer
********************************************************/
void displayCurrentPlayer()
{
    char turnMessage[50];
    sprintf(turnMessage, "Player %c's Turn", player);
    DrawText(turnMessage, 375, 75, 30, RAYWHITE);
}
