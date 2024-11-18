/* Define preprocessor statements */
#include "game_logic.h"


int player1Score = 0;      // Track Player 1's score
int player2Score = 0;      // Track Player 2's score
int gameEnded = 0;         // Track if the game has ended, 0 means game has not end
char winner = ' ';         // Store the winner symbol ('O' or 'X'), empty if no winner
int scoreUpdated = 0;      // Track if the score has been updated for current game

/********************************************************
function: print_board
    displays the tic tac toe board in console

input: board - 3 by 3 2D character array
********************************************************/
void print_board(char board[3][3])
{
    printf("\n\n");
    printf("%c | %c | %c\n", board[0][0], board[0][1], board[0][2]);    /* Prints first row of board into console */
    printf("%c | %c | %c\n", board[1][0], board[1][1], board[1][2]);    /* Prints second row of board into console */
    printf("%c | %c | %c\n", board[2][0], board[2][1], board[2][2]);    /* Prints third row of board into console */
    printf("\n\n");
}

/********************************************************
function: check_board_status
    check the board for a winner or loser

input: board - 3 by 3 2D character array

return: status - integer 
********************************************************/
int check_board_status(char board[3][3])
{


    if ((board[0][0] == board[0][1]) && (board[0][1] == board[0][2]) && (board[0][0] != '-'))       /* Checks if top row is not empty("-") and is populated by same player */
    {
        winner = (board[0][0] == PLAYER1) ? PLAYER1 : PLAYER2;
        return STATE_WIN;
    }
    if ((board[1][0] == board[1][1]) && (board[1][1] == board[1][2]) && (board[1][0] != '-'))       /* Checks if middle row is not empty("-") and is populated by same player */
    {
        winner = (board[1][0] == PLAYER1) ? PLAYER1 : PLAYER2;  // Set winner based on row value
        return STATE_WIN;
    }
    if ((board[2][0] == board[2][1]) && (board[2][1] == board[2][2]) && (board[2][0] != '-'))       /* Checks if bottom row is not empty("-") and is populated by same player */
    {
        winner = (board[2][0] == PLAYER1) ? PLAYER1 : PLAYER2;  // Set winner based on row value
        return STATE_WIN;
    }
    if ((board[0][0] == board[1][0]) && (board[1][0] == board[2][0]) && (board[0][0] != '-'))       /* Checks if left column is not empty("-") and is populated by same player */
    {
        winner = (board[0][0] == PLAYER1) ? PLAYER1 : PLAYER2;  // Set winner based on column value
        return STATE_WIN;
    }
    if ((board[0][1] == board[1][1]) && (board[1][1] == board[2][1]) && (board[0][1] != '-'))       /* Checks if middle column is not empty("-") and is populated by same player */
    {
        winner = (board[0][1] == PLAYER1) ? PLAYER1 : PLAYER2;  // Set winner based on column value
        return STATE_WIN;
    }
    if ((board[0][2] == board[1][2]) && (board[1][2] == board[2][2]) && (board[0][2] != '-'))       /* Checks if right column is not empty("-") and is populated by same player */
    {
        winner = (board[0][2] == PLAYER1) ? PLAYER1 : PLAYER2;  // Set winner based on column value
        return STATE_WIN;
    }
    if ((board[0][0] == board[1][1]) && (board[1][1] == board[2][2]) && (board[0][0] != '-'))       /* Checks if top left to bottom right diagonal line is not empty("-") and is populated by same player */
    {
        winner = (board[0][0] == PLAYER1) ? PLAYER1 : PLAYER2;  // Set winner based on diagonal value
        return STATE_WIN;
    }
    if ((board[0][2] == board[1][1]) && (board[1][1] == board[2][0]) && (board[0][2] != '-'))       /* Checks if bottom left to top right diagonal line is not empty("-") and is populated by same player */
    {
        winner = (board[0][2] == PLAYER1) ? PLAYER1 : PLAYER2;  // Set winner based on diagonal value
        return STATE_WIN;
    }
        
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == '-')         /* Checks if there is empty space in board */
                {

                    return STATE_PLAYING;   /* There is still empty spaces in the board, play on. Break out of loop by returning status, which is 0*/
                }
            }

        }

        return STATE_DRAW;  /* There are no empty space in board. Set status to 1, indicating a tie */

}

/********************************************************
function: update_board
    updates the tic tac toe game board

inputs: board - 3 by 3 2D character array
        row - integer row to be updated
        col - integer column to be updated
        curr_player - player to be updated
********************************************************/
void update_board(char board[3][3], int row, int col, char curr_player)
{
    if (board[row][col] == '-')
    {
        board[row][col] = curr_player;
    }
}

/********************************************************
function: restartBoard
    resets board for a new game

inputs: board
********************************************************/
void restartBoard(char board[3][3]) 
{
    int gameState = STATE_MENU;
    gameState = check_board_status(board);

    if (gameState == STATE_WIN || gameState == STATE_DRAW) 
    {
        for (int i = 0; i < 3; i++) 
        {
            for (int j = 0; j < 3; j++) 
            {
                board[i][j] = '-';
            }
        }
        gameState = STATE_PLAYING;
        gameEnded = 0;
        scoreUpdated = 0; /*Reset scoreUpdated for the new game*/
        winner = ' ';
    }
}

/********************************************************
function: scoreBoard
    update scoreboard based on winner
********************************************************/
void scoreBoard() 
{
    if (gameState == STATE_WIN && scoreUpdated == 0) 
    {
        if (winner == PLAYER1) 
        {
            player1Score++;
        } 
        else if (winner == PLAYER2) 
        {
            player2Score++;
        }
        scoreUpdated = 1;
    }
}


