/* Define preprocessor statements */
#include <stdio.h>
#define PLAYER1 'O'
#define PLAYER2 'X'


/********************************************************
function: print_board
    displays the tic tac toe board

Input: board - array with size of 9

********************************************************/
void print_board(char board[9])
{
    printf("\n\n");
    printf("%c | %c | %c\n", board[0], board[1], board[2]);
    printf("%c | %c | %c\n", board[3], board[4], board[5]);
    printf("%c | %c | %c\n", board[6], board[7], board[8]);
    printf("\n\n");
}

/********************************************************
function: check_board_status
    check the board for a winner or loser

Input: board - array with size of 9
Return: status - integer 0, 1 or 2. 0 - play on, 1 - tie, 2 - win

********************************************************/
int check_board_status(char board[9])
{
    /* Initialise variables */
    int status = 0;  /*  status of game. 0 indicates play on, 1 indicates tie, 2 indicates win */


    if (((board[0] == board[1]) && (board[1] == board[2]) && (board[0] != '-')) ||   /* Checks if top row is not empty("-") and is populated by same player */
        ((board[3] == board[4]) && (board[4] == board[5]) && (board[3] != '-')) ||   /* Checks if middle row is not empty("-") and is populated by same player */
        ((board[6] == board[7]) && (board[7] == board[8]) && (board[6] != '-')) ||   /* Checks if bottom row is not empty("-") and is populated by same player */
        ((board[0] == board[3]) && (board[3] == board[6]) && (board[0] != '-')) ||   /* Checks if left column is not empty("-") and is populated by same player */
        ((board[1] == board[4]) && (board[4] == board[7]) && (board[1] != '-')) ||   /* Checks if middle column is not empty("-") and is populated by same player */
        ((board[2] == board[5]) && (board[5] == board[8]) && (board[2] != '-')) ||   /* Checks if right column is not empty("-") and is populated by same player */
        ((board[0] == board[4]) && (board[4] == board[8]) && (board[0] != '-')) ||   /* Checks if top left to bottom right diagonal line is not empty("-") and is populated by same player */
        ((board[2] == board[4]) && (board[4] == board[6]) && (board[2] != '-')))     /* Checks if bottom left to top right diagonal line is not empty("-") and is populated by same player */

        {
            status = 2;  /* Ding ding! There's a winner. Status is set to 2*/
        }

    else
    {
        for (int i = 0; i < 9; i++)
        {
            if (board[i] == '-')   /* Checks if there is empty space in board */
            {
                return status;  /* There is still empty spaces in the board, play on. Break out of loop by returning status, which is 0*/
            }
        }

        status = 1;  /* There are no empty space in board. Set status to 1, indicating a tie */
    }

    return status;  /* Function returns status */
}

void printWinner(char winner) /*To announce winner or a tie*/
{
    if (winner == PLAYER1)
    {
        printf("Player O WINS!\n");
    }
    else if (winner == PLAYER2)
    {
        printf("Player X WINS!\n");
    }
    else
    {
        printf("IT'S A TIE!\n");
    }
}

