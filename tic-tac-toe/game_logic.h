/* Define preprocessor statements */

/* Define include guards. Ensures contents of this file does not get included more than once */
#ifndef GAME_LOGIC_H    /* Run the following if GAME_LOGIC_H has not been defined */
#define GAME_LOGIC_H    /* Defines GAME_LOGIC_H*/

#include <stdio.h>
#include "gui.h"
#define PLAYER1 'O'
#define PLAYER2 'X'

extern int player1Score;
extern int player2Score;
extern int gameEnded;
extern char winner;

void print_board(char board[3][3]);
int check_board_status(char board[3][3]);
void update_board(char board[3][3], int row, int col, char curr_player);
void restartBoard(char board[3][3]);
void scoreBoard();



#endif
