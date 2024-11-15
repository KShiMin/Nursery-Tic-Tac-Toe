#ifndef gamelogic_h
#define gamelogic_h

extern int player1Score;
extern int player2Score;
extern int gameEnded;
extern char winner;

void print_board(char board[3][3]);
int check_board_status(char board[3][3]);
void printWinner(char winner);
void update_board(char board[3][3], int row, int col, char curr_player);
void restartBoard(char board[3][3]);
void scoreBoard();

#endif
