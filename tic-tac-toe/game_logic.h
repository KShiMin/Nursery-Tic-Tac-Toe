/* Define include guards. Ensures contents of this file does not get included more than once*/
#ifndef GAME_LOGIC_H  /* Run the following if GAME_LOGIC_H has not been defined */
#define GAME_LOGIC_H  /* Defines GAME_LOGIC_H */




/* Declare global variables of player_vs_cpu.c file*/
void print_board(char board[3][3]);
int check_board_status(char board[3][3]);
void printWinner(char winner);
void update_board(char board[3][3], int row, int col, char curr_player);




#endif  /* End of header file */
