/* Define preprocessor */
#include <stdio.h>
#include "game_logic.h"

/********************************************************
function: playPlayer

Input:

********************************************************/
void playPlayer()
{
    printf("\n\nSelected game mode: Player Vs Player");

    char board[9] = {'-', '-', '-', '-', '-', '-', '-', '-', '-'};
    char current_player = 'X';
    char winner = ' ';
    int move = 0;
    int status = 0;

    do
    {
        print_board(board);
        printf("Player %c, enter box (0-8): ", current_player);
        while (scanf("%i", &move) != 1)
        {
            printf("Invalid input! Please enter a number 0-8: ");
            while (getchar() != '\n');
        }
        board[move] = current_player;
        status = check_board_status(board);
        if (status == 2)
        {
            winner = current_player;
            break;
        }
        current_player = (current_player == 'X') ? 'O' : 'X';
        
    } while (status == 0);
    print_board(board);
    printWinner(winner);
}


/*
char board[9] = {'-', '-', '-', '-', '-', '-', '-', '-', '-'};
char current_player = 'X';
int move = 0;
int status = 0;
while (status == 0)
{
    print_board(board);
    scanf("%i", &move);
    board[move] = current_player;
    status = check_board_status(board);
    current_player = (current_player == 'X') ? 'O' : 'X';
    
}
print_board(board);
*/
