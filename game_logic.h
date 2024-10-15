/* Define include guards. Ensures contents of this file does not get included more than once*/
#ifndef GAME_LOGIC_H  /* Run the following if PLAYER_VS_CPU_H has not been defined */
#define GAME_LOGIC_H  /* Defines PLAYER_VS_PLAYER */

/* Declare global variables of player_vs_cpu.c file*/
void print_board(char board[9]);
int check_board_status(char board[9]);

#endif  /* End of header file */
