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
void print_board(char board[BOARD_SIZE][BOARD_SIZE]) {
    printf("\n\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%c | %c | %c\n", board[i][0], board[i][1], board[i][2]);
    }
    printf("\n\n");
}

/********************************************************
function: check_board_status
    check the board for a winner or loser

input: board - 3 by 3 2D character array

return: status - integer 
********************************************************/
int check_board_status(char board[BOARD_SIZE][BOARD_SIZE]) {
    // Check rows and columns for win
    for (int i = 0; i < BOARD_SIZE; i++) {
        if ((board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != EMPTY) ||
            (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != EMPTY)) {
            winner = (board[i][i] == PLAYER1) ? PLAYER1 : PLAYER2;
            return STATE_WIN;
        }
    }

    // Check diagonals for win
    if ((board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != EMPTY) ||
        (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != EMPTY)) {
        winner = (board[1][1] == PLAYER1) ? PLAYER1 : PLAYER2;
        return STATE_WIN;
    }

    // Check for empty spaces
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == EMPTY) {
                return STATE_PLAYING; // Game is still ongoing
            }
        }
    }

    return STATE_DRAW; // No empty spaces, it's a draw
}

/********************************************************
function: update_board
    updates the tic tac toe game board

inputs: board - 3 by 3 2D character array
        row - integer row to be updated
        col - integer column to be updated
        curr_player - player to be updated
********************************************************/
void update_board(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char curr_player)
{
    if (board[row][col] == EMPTY)
    {
        board[row][col] = curr_player;
    }
}

/********************************************************
function: restartBoard
    resets board for a new game

inputs: board
********************************************************/
void restartBoard(char board[BOARD_SIZE][BOARD_SIZE]) 
{
    int gameState;
    gameState = check_board_status(board);

    if (gameState == STATE_WIN || gameState == STATE_DRAW) 
    {
        for (int i = 0; i < BOARD_SIZE; i++) 
        {
            for (int j = 0; j < BOARD_SIZE; j++) 
            {
                board[i][j] = '-';
            }
        }
        gameEnded = 0;
        scoreUpdated = 0; /*Reset scoreUpdated for the new game*/
        winner = ' ';
        // Always reset to O_PLAYER starting first
        // player = O_PLAYER;
        time_spent = 0.0; // Reset time_spent for the new game
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

