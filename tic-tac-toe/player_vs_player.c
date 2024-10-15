/* Define preprocessor */
#include <stdio.h>

/********************************************************
function: playPlayer

Input:

********************************************************/
void playPlayer()
{
    printf("\n\nSelected game mode: Player Vs Player");
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
