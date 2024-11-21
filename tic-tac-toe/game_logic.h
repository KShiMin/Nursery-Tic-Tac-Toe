/* Define preprocessor statements */

/* Define include guards. Ensures contents of this file does not get included more than once */
#ifndef GAME_LOGIC_H    /* Run the following if GAME_LOGIC_H has not been defined */
#define GAME_LOGIC_H    /* Defines GAME_LOGIC_H*/

#include <stdio.h>
#include "gui.h"
#define PLAYER1 'O'
#define PLAYER2 'X'
#define EMPTY '-'
#define BOARD_SIZE 3

extern int player1Score;
extern int player2Score;
extern int gameEnded;
extern char winner;

void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
int check_board_status(char board[BOARD_SIZE][BOARD_SIZE]);
void update_board(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char curr_player);
void restartBoard(char board[BOARD_SIZE][BOARD_SIZE]);
void scoreBoard();



#endif
