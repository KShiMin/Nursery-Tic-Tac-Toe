#include "gui.h"
#include "game_logic.h"
#include <stdio.h>


char player = X_PLAYER;
int gameMode = PVP;
int gameState = STATE_MENU;
char board[3][3] = {'-', '-', '-', '-', '-', '-', '-', '-', '-'};

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

    else if (gameState == STATE_PLAYING)
    {
        draw_grid();
        draw_markers();
        gameState = check_board_status(board);
        
        if (gameState == STATE_PLAYING)
        {
            CheckMouseInput();
            gameState = check_board_status(board);

            if (gameState == STATE_PLAYING)
            {
                if (gameMode == PVC && player == O_PLAYER)
                {
                    /* Call minimax algorithm! */
                }

            }

        }


    }

    else
    {
        draw_grid();
        draw_markers();
        game_over();
    }
        
}

/********************************************************
function: game_over



********************************************************/
void game_over()
{
    Rectangle display = {300, 400, 400, 200};

    if (gameState == STATE_WIN)
    {
        const char *text = (player == O_PLAYER) ? "X Wins" : "O Wins!";
        DrawRectangleRec(display, LIGHTGRAY);
        DrawText(text, 425, 450, 50, BLACK);
    }

    else if (gameState == STATE_DRAW)
    {
        DrawRectangleRec(display, LIGHTGRAY);
        DrawText("DRAW", 425, 450, 50, BLACK);
    }

}

/********************************************************
function: draw_menu



********************************************************/
void draw_menu()
{
    DrawText("Tic Tac Toe", 380, 150, 30, DARKGRAY);

    /* PvP button */
    Rectangle pvpButton = {380, 250, 200, 50};
    DrawRectangleRec(pvpButton, LIGHTGRAY);
    DrawText("Player vs Player", 400, 265, 20, BLACK);

    /* CPU button */
    Rectangle cpuButton = {380, 350, 200, 50};
    DrawRectangleRec(cpuButton, LIGHTGRAY);
    DrawText("Player vs CPU", 400, 365, 20, BLACK);

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
                update_board(board, row, col, player);
            }
            
            print_board(board);

            player = (player == X_PLAYER) ? O_PLAYER : X_PLAYER;
            
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
    DrawCircle(x, y, GRID_OFFSET/2 - 1, BLACK);
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