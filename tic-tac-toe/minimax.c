/*Delete this line*/
/* Define preprocessor statements */
#include "minimax.h"

/********************************************************************************
function: ai  
    duplicate actual board for minimax() to run simulation
    copy duplicated board content back to actual board once computer made a move

    Input:
    board[3][3] - game board
    num_wins - number of wins and draws for ai
    difficulty - difficulty level of ai
********************************************************************************/
void ai(char board[3][3], int num_wins, int difficulty) {
    // Convert the char board to GameState format
    GameState gameState;
    gameState.wins = num_wins;
    gameState.currentPlayer = PLAYER_X;  // CPU is X
    gameState.gameOver = false;          // game ongoing

    // // Convert char board to integer board
    // for(int i = 0; i < BOARD_SIZE; i++) {
    //     for(int j = 0; j < BOARD_SIZE; j++) {
    //         if(board[i][j] == 'X') {
    //             gameState.board[i][j] = PLAYER_X;
    //         } else if(board[i][j] == 'O') {
    //             gameState.board[i][j] = PLAYER_O;
    //         } else {
    //             gameState.board[i][j] = EMPTY;
    //         }
    //     }
    // }

    // Copy board to gameState board
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            gameState.board[i][j] = board[i][j];
        }
    }
    
    mmMove(&gameState);

    // // Convert integer board to char board
    // for(int i = 0; i < BOARD_SIZE; i++) {
    //     for(int j = 0; j < BOARD_SIZE; j++) {
    //         if(gameState.board[i][j] == PLAYER_X) {
    //             board[i][j] = 'X';
    //         } else if(gameState.board[i][j] == PLAYER_O) {
    //             board[i][j] = 'O';
    //         }
    //     }
    // }

    // Copy board back to original board
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = gameState.board[i][j];
        }
    }

}

/********************************************************
function: displayBoard
    displays the board for troubleshooting

    Input:
    game - Pointer to access all variables in GameState

    Return:
    Print out board with updated cells
********************************************************/
void displayBoard(GameState *game) {
    printf("\n");
    printf("\n   +---+---+---+\n");
    // Check every cell content
    for(int i = 0; i < BOARD_SIZE; i++) {
        printf("   |");
        for(int j = 0; j < BOARD_SIZE; j++) {
            // If cell is empty
            char symbol = ' ';
            // If cell marked by computer
            if(game->board[i][j] == PLAYER_X) symbol = 'X';
            // If cell marked by player
            if(game->board[i][j] == PLAYER_O) symbol = 'O';
            printf(" %c |", symbol);
        }
        printf("\n");
        printf("   +---+---+---+\n");
    }
    printf("\n");
}

/********************************************************
function: mmMove
    make a move on board
    switch players if game not concluded

    Input:
    game - Pointer to access all variables in GameState
    row - row of selected cell
    col - column of selected cell

    Return:
    boolean value - is selected cell a valid move
********************************************************/
bool makeMove(GameState *game, int row, int col) {
    // Check if selected cell is valid move
    if (game->board[row][col] == EMPTY && !game->gameOver) {
        // Mark cell with current player's symbol
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
        // Switch players
        else {
            game->currentPlayer = (game->currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
        }
        return true;
    }
    return false;
}

/********************************************************
function: checkWin
    checks for winning conditions

    Inputs:
    game - Pointer to access all variables in GameState
    players - player symnbol 'X' or 'O'

    Return:
    boolean value - has winning conditions been met
********************************************************/
bool checkWin(GameState *game, unsigned char player) {
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

/*******************************************************************
function: isBoardFull
    check for empty cells

    Input:
    game - Pointer to access all variables in GameState

    Return:
    boolean value - if cell is empty
********************************************************************/
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

/*******************************************************************
function: minimax
    simulates all possible moves

    Input:
    game - Pointer to access all variables in GameState
    depth - turns in a game
    alpha - 
    beta - 
    isMaximizing - tell minimax() to simulate for computer or player

    Return:
    bestScore - highest score from all possible moves
********************************************************************/
int minimax(GameState* game, int depth, int alpha, int beta, bool isMaximizing) {
    // Check if player wins
    if(checkWin(game, PLAYER_O)) return -10 + depth;
    // Check if computer wins
    if(checkWin(game, PLAYER_X)) return 10 - depth;
    // Check if no empty cells, draw
    if(isBoardFull(game)) return 0;

    // Maximise score for computer
    if(isMaximizing) {
        int bestScore = -1000;
        // Check every empty cell on board
        for(int i = 0; i < BOARD_SIZE; i++) {
            for(int j = 0; j < BOARD_SIZE; j++) {
                if(game->board[i][j] == EMPTY) {
                    // Simulate computer moves
                    game->board[i][j] = PLAYER_X;
                    // Recursively call minimax until game concludes
                    int score = minimax(game, depth + 1, alpha, beta, false);
                    // Undo simulated moves
                    game->board[i][j] = EMPTY;
                    //Evaluate best score
                    bestScore = (score > bestScore) ? score : bestScore;
                    alpha = (alpha > bestScore) ? alpha : bestScore;
                    
                    // Alpha-beta pruning
                    if(beta <= alpha) {
                        break;
                    }
                }
            }
        }
        return bestScore;
    }
    // Minimise score for player
    else {
        int bestScore = 1000;
        // Check every empty cell on board
        for(int i = 0; i < BOARD_SIZE; i++) {
            for(int j = 0; j < BOARD_SIZE; j++) {
                if(game->board[i][j] == EMPTY) {
                    // Simulate player moves
                    game->board[i][j] = PLAYER_O;
                    // Recursively call minimax until game concludes
                    int score = minimax(game, depth + 1, alpha, beta, true);
                    // Undo simulated moves
                    game->board[i][j] = EMPTY;
                    //Evaluate best score
                    bestScore = (score < bestScore) ? score : bestScore;
                    beta = (beta < bestScore) ? beta : bestScore;
                    
                    // Alpha-beta pruning
                    if(beta <= alpha) {
                        break;
                    }
                }
            }
        }
        return bestScore;
    }
}

// int minimax(GameState *game, int depth, bool isMaximizing) {
//     // Check if player O wins
//     if(checkWin(game, PLAYER_O)) return -10 + depth;
//     // Check if player X wins
//     if(checkWin(game, PLAYER_X)) return 10 - depth;
//     // Check if no empty cells
//     if(isBoardFull(game)) return 0;

//     if(isMaximizing) {
//         int bestScore = -1000;
//         for(int i = 0; i < BOARD_SIZE; i++) {
//             for(int j = 0; j < BOARD_SIZE; j++) {
//                 if(game->board[i][j] == EMPTY) {
//                     game->board[i][j] = PLAYER_X;
//                     int score = minimax(game, depth + 1, false);
//                     game->board[i][j] = EMPTY;
//                     bestScore = (score > bestScore) ? score : bestScore;
//                 }
//             }
//         }
//         return bestScore;
//     }
//     else {
//         int bestScore = 1000;
//         for(int i = 0; i < BOARD_SIZE; i++) {
//             for(int j = 0; j < BOARD_SIZE; j++) {
//                 if(game->board[i][j] == EMPTY) {
//                     game->board[i][j] = PLAYER_O;
//                     int score = minimax(game, depth + 1, true);
//                     game->board[i][j] = EMPTY;
//                     bestScore = (score < bestScore) ? score : bestScore;
//                 }
//             }
//         }
//         return bestScore;
//     }
// }


/*******************************************************************
function: mmMove
    simulates all possible moves
    find best and second-best moves
    decide which move to use

    Input:
    game - Pointer to access all variables in GameState
********************************************************************/
void mmMove(GameState *game) {
    // Seed random number generator once at the start
    srand(time(0));  

    int bestScore = -1000;
    int secondBestScore = -1000;    
    int bestRow = -1;
    int bestCol = -1;
    int secondBestRow = -1;
    int secondBestCol = -1;

    // Check every empty cell on board
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            if(game->board[i][j] == EMPTY) {
                // Simulate computer moves
                game->board[i][j] = PLAYER_X;
                // Recursively call minimax until game concludes
                int score = minimax(game, 0, -1000, 1000, false);
                // Undo simulated move
                game->board[i][j] = EMPTY;

                // if score is the highest
                if(score > bestScore) {
                    // second-best becomes the lesser scoring move
                    secondBestScore = bestScore;
                    secondBestRow = bestRow;
                    secondBestCol = bestCol;

                    // best becomes the higher scoring move
                    bestScore = score;
                    bestRow = i;
                    bestCol = j;
                }
                
                // if score is second highest
                else if (score > secondBestScore) {
                    // setting score to be second best move
                    secondBestScore = score;
                    secondBestRow = i;
                    secondBestCol = j;
                }
            }
        }
    }
    


    // Decide to use best or second-best move
    if (rand() % 100 >= difficulty && secondBestScore != -1000) { 
        bestRow = secondBestRow;
        bestCol = secondBestCol;
    }

    // Make the decided move
    if(bestRow != -1 && bestCol != -1) {
        makeMove(game, bestRow, bestCol);
    }

}

// void mmMove(GameState *game) {
//     srand(time(0));  // Seed random number generator once at the start
//     int bestScore = -1000;
//     int secondBestScore = -1000;
//     int bestRow = -1;
//     int bestCol = -1;
//     int secondBestRow = -1;
//     int secondBestCol = -1;

//     for(int i = 0; i < BOARD_SIZE; i++) {
//         for(int j = 0; j < BOARD_SIZE; j++) {
//             // Check if each cell is empty
//             if(game->board[i][j] == EMPTY) {
//                 // If this cell is empty =  ai simulate making their move there
//                 game->board[i][j] = PLAYER_X;
//                 // Calculate score
//                 int score = minimax(game, 0, false);
//                 // Undo the move
//                 game->board[i][j] = EMPTY;

//                 // Update best and second-best score
//                 if(score > bestScore) {
//                     // second best becomes the lesser scoring move
//                     secondBestScore = bestScore;
//                     secondBestRow = bestRow;
//                     secondBestCol = bestCol;

//                     // best becomes the higher scoring move
//                     bestScore = score;
//                     bestRow = i;
//                     bestCol = j;
//                 }
//                 // setting second best to be second best option 
//                 else if (score > secondBestScore) {
//                     secondBestScore = score;
//                     secondBestRow = i;
//                     secondBestCol = j;
//                 }
//             }
//         }
//     }
    


//     // Decide based on difficulty to skip the best move and use the second-best one
//     if (rand() % 100 >= difficulty && secondBestScore != -1000) { // chance to choose second best
//         bestRow = secondBestRow;
//         bestCol = secondBestCol;
//     }


//     if(bestRow != -1 && bestCol != -1) {
//         makeMove(game, bestRow, bestCol);
//     }

// }