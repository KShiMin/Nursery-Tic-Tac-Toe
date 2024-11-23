/* Define include guards. Ensures contents of this file does not get included more than once */
#ifndef Q_LEARNING  // This will run if Q_LEARNING has not been defined
#define Q_LEARNING  // Defines Q_LEARNING

#include <stdbool.h>
#include <stdio.h> // Required for printf
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * q_learning.h: Header file for Q-learning Implementation 
 * 
 * This file contains constants, macros, structures, enumerations, and function
 * prototypes for implementing the Q-learning logic, game mechanics, and player interactions.
 * 
 */

// Debugging Macro
// Enables debug message when DEBUG is 1, otherwise debug messages are suppressed
#define DEBUG 1
#if DEBUG
    #define DEBUG_PRINT(fmt,...) printf(fmt, ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt,...)
#endif

// Constant
#define MAX_STRINGS 10000 // Maximum number of states stored in the player's state array
#define MAX_LENGTH 9 // Length of each state array when flatten (3x3)
#define QTABLE_LENGTH 5000 // Maximum number of entries in Q-table

// Learning Parameters
extern const float LR;      // Learning rate for Q-value updates
extern const float DECAY;   // Decary factor for exploration rate over episodes

// Enumerations
// Represents the type of player: HUMAN, CPU, or an empty board cell
typedef enum { BOARD_BLANK = 0, HUMAN = 1, CPU = -1 } PlayerType; // Player type integer definition


// Structures
// Represents a coordinate on the board (row and column)
typedef struct{
    int row, col;
} Coord;

// Represents a single Q-value entry with a state key and its associated value
typedef struct{
    int key[MAX_LENGTH];    //Key representing the board state in a flatten array
    float val;              // Q-value associated with the state
} Qvalue;

// Represents a player with their state history, Q-table, and exploration rate
typedef struct{
    int state[MAX_STRINGS][MAX_LENGTH]; // Player's recorded state
    Qvalue *state_val[QTABLE_LENGTH];   // Pointer to player's Q-table
    float exp_rate;                     // Exploration rate for Q-learning
} Player;

// Represents the overall game, including players, game status, and the current turn
typedef struct{
    bool game_status;   // True if game has ended, false otherwise
    Player p1, p2;      // Players in the game
    int playing;        // Indicates the current player (HUMAN Or CPU)
} Game;

// Function prototypes
void initPlayer(Player *player, float exp_rate);
int startingPlayer();
void reset(Player player[2], int board[3][3]);
int availPos(int board[3][3], Coord availCoord[9]);
void defaultQValue(Qvalue* q_table[QTABLE_LENGTH], int state[]);
int findQValue(int state[MAX_LENGTH], Qvalue *q_table[QTABLE_LENGTH]);
void updateQtable(Player* player, int winner);
void updateBoardState(int board[3][3], Coord action, Game *game);
int check_win(int board[3][3], Game *game);
void saveQTable(Qvalue *q_table[QTABLE_LENGTH], const char *filename);
void loadQTable(Qvalue *q_table[QTABLE_LENGTH], const char *filename);
void trainModel(int episode, int board[3][3]);
void pve(int board[3][3]);
Coord guiMLmove(char board[3][3]);


#endif