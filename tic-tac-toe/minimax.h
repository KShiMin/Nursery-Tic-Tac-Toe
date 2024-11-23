/* Define include guards. Ensures contents of this file does not get included more than once*/
#ifndef MINIMAX_H  /* Run the following if MINIMAX_H has not been defined */
#define MINIMAX_H  /* Defines PLAYER_VS_PLAYER */

#include <stdio.h>
#include <stdlib.h> // For rand()
#include <time.h>   // For seeding random
#include <stdbool.h>
#include "game_logic.h"
#include "gui.h"



#define BOARD_SIZE 3
#define EMPTY '-'
#define PLAYER_O 1
#define PLAYER_X 2

extern int previousNumWins;
extern int difficulty;

typedef struct {
    int board[BOARD_SIZE][BOARD_SIZE];
    int currentPlayer;
    bool gameOver;
    int winner; // If winner = 0, draw, if winner = player --> player wins
    int wins;
    //int difficulty;
} GameState;

void displayBoard(GameState* game);
bool checkWin(GameState* game, int player);
bool isBoardFull(GameState* game);
bool makeMove(GameState* game, int row, int col);
int minimax(GameState* game, int depth, bool isMaximizing);
void mmMove(GameState* game);
void ai(char board[3][3], int num_wins, int difficulty);

#endif  /* End of header file */