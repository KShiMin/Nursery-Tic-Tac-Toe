/* Define preprocessor statements */

#include "minimax.h"


int training = 1; // Need to train Q-learning model first

void ai(char board[3][3], int num_wins, int difficulty) {
    // Convert the char board to GameState format
    GameState gameState;
    gameState.wins = num_wins;
    gameState.currentPlayer = PLAYER_X;  // CPU is X
    gameState.gameOver = false;          // game ongoing

    double time_spent=0.0;      /*to calculate time for algorithm*/
    
    // For debugging
    // printf("Difficulty is %d%%\n", difficulty);
    // printf("PreviousNumWins is %d\n", previousNumWins);

    // Convert char board to integer board
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            if(board[i][j] == 'X') {
                gameState.board[i][j] = PLAYER_X;
            } else if(board[i][j] == 'O') {
                gameState.board[i][j] = PLAYER_O;
            } else {
                gameState.board[i][j] = EMPTY;
            }
        }
    }
    
    clock_t begin =clock();     /*start timing*/
    mmMove(&gameState);
    clock_t end = clock();      /*end timing*/
    time_spent += (double)(end-begin)/CLOCKS_PER_SEC;
    printf("\n Time for CPU to make a move is %f seconds\n",time_spent);

    // Convert integer board to char board
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            if(gameState.board[i][j] == PLAYER_X) {
                board[i][j] = 'X';
            } else if(gameState.board[i][j] == PLAYER_O) {
                board[i][j] = 'O';
            }
        }
    }

}

// display the board
void displayBoard(GameState *game) {
    printf("\n");
    printf("\n   +---+---+---+\n");
    for(int i = 0; i < BOARD_SIZE; i++) {
        printf("   |");
        for(int j = 0; j < BOARD_SIZE; j++) {
            char symbol = ' ';
            if(game->board[i][j] == PLAYER_X) symbol = 'X';
            if(game->board[i][j] == PLAYER_O) symbol = 'O';
            printf(" %c |", symbol);
        }
        printf("\n");
        printf("   +---+---+---+\n");
    }
    printf("\n");
}

// Save Human player's move and check winning condiitons
bool makeMove(GameState *game, int row, int col) {
    if (game->board[row][col] == EMPTY && !game->gameOver) {
        game->board[row][col] = game->currentPlayer;

        // Display board after move
        displayBoard(game);

        // Check for win
        if (checkWin(game, game->currentPlayer)) {
            game->gameOver = true;
            game->winner = game->currentPlayer;
        }
        // Check for draw
        else if (isBoardFull(game)) {
            game->gameOver = true;
            game->winner = 0;
        }
        else {
            // Switch players
            game->currentPlayer = (game->currentPlayer == PLAYER_X) ? 
                PLAYER_O : PLAYER_X;
        }
        return true;
    }
    return false;
}

// Winning Conditions
bool checkWin(GameState *game, int player) {
    // Check rows = current player symbol
    for(int i = 0; i < BOARD_SIZE; i++) {
        if(game->board[i][0] == player &&
           game->board[i][1] == player &&
           game->board[i][2] == player) {
            return true;
        }
    }

    // Check columns = current player symbol
    for(int i = 0; i < BOARD_SIZE; i++) {
        if(game->board[0][i] == player &&
           game->board[1][i] == player &&
           game->board[2][i] == player) {
            return true;
        }
    }

    // Check diagonals = current player symbol
    if(game->board[0][0] == player &&
       game->board[1][1] == player &&
       game->board[2][2] == player) {
        return true;
    }

    if(game->board[0][2] == player &&
       game->board[1][1] == player &&
       game->board[2][0] == player) {
        return true;
    }

    return false;
}

// Check if no empty cells
bool isBoardFull(GameState *game) {
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            if(game->board[i][j] == EMPTY) {
                return false;
            }
        }
    }
    return true;
}

int minimax(GameState *game, int depth, bool isMaximizing) {
    // Check if player O wins
    if(checkWin(game, PLAYER_O)) return -10 + depth;
    // Check if player X wins
    if(checkWin(game, PLAYER_X)) return 10 - depth;
    // Check if no empty cells
    if(isBoardFull(game)) return 0;

    if(isMaximizing) {
        int bestScore = -1000;
        for(int i = 0; i < BOARD_SIZE; i++) {
            for(int j = 0; j < BOARD_SIZE; j++) {
                if(game->board[i][j] == EMPTY) {
                    game->board[i][j] = PLAYER_X;
                    int score = minimax(game, depth + 1, false);
                    game->board[i][j] = EMPTY;
                    bestScore = (score > bestScore) ? score : bestScore;
                }
            }
        }
        return bestScore;
    }
    else {
        int bestScore = 1000;
        for(int i = 0; i < BOARD_SIZE; i++) {
            for(int j = 0; j < BOARD_SIZE; j++) {
                if(game->board[i][j] == EMPTY) {
                    game->board[i][j] = PLAYER_O;
                    int score = minimax(game, depth + 1, true);
                    game->board[i][j] = EMPTY;
                    bestScore = (score < bestScore) ? score : bestScore;
                }
            }
        }
        return bestScore;
    }
}

void mmMove(GameState *game) {
    srand(time(0));  // Seed random number generator once at the start
    int bestScore = -1000;
    int secondBestScore = -1000;
    int bestRow = -1;
    int bestCol = -1;
    int secondBestRow = -1;
    int secondBestCol = -1;

    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            // Check if each cell is empty
            if(game->board[i][j] == EMPTY) {
                // If this cell is empty =  ai simulate making their move there
                game->board[i][j] = PLAYER_X;
                // Calculate score
                int score = minimax(game, 0, false);
                // Undo the move
                game->board[i][j] = EMPTY;

                // Update best and second-best score
                if(score > bestScore) {
                    // second best becomes the lesser scoring move
                    secondBestScore = bestScore;
                    secondBestRow = bestRow;
                    secondBestCol = bestCol;

                    // best becomes the higher scoring move
                    bestScore = score;
                    bestRow = i;
                    bestCol = j;
                }
                // setting second best to be second best option 
                else if (score > secondBestScore) {
                    secondBestScore = score;
                    secondBestRow = i;
                    secondBestCol = j;
                }
            }
        }
    }
    


    // Decide based on difficulty to skip the best move and use the second-best one
    if (rand() % 100 >= difficulty && secondBestScore != -1000) { // chance to choose second best
        bestRow = secondBestRow;
        bestCol = secondBestCol;
    }


    if(bestRow != -1 && bestCol != -1) {
        makeMove(game, bestRow, bestCol);
    }

}