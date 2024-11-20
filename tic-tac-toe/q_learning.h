#ifndef Q_LEARNING
#define Q_LEARNING

#include <stdbool.h>
#include <stdio.h> // Required for printf

// Debugging Macro
#define DEBUG 1
#if DEBUG
    #define DEBUG_PRINT(fmt,...) printf(fmt, ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt,...)
#endif

// Constant
#define MAX_STRINGS 10000 // num of string array can hold
#define MAX_LENGTH 9 // how long of a string it can hold
#define QTABLE_LENGTH 5000 // max possible amount of states in q table

// Learning Parameters
extern const float LR;
extern const float DECAY;

// Enumerations
typedef enum { BLANK = 0, HUMAN = 1, CPU = -1} PlayerType; // Player type integer definition


// Structures
// defining own coordinates
typedef struct{
    int row, col;
} Coord;

typedef struct{
    int key[MAX_LENGTH];
    float val;
} Qvalue;

typedef struct{
    int state[MAX_STRINGS][MAX_LENGTH];
    Qvalue *state_val[QTABLE_LENGTH];
    float exp_rate;
} Player;

typedef struct{
    bool game_status;
    Player p1, p2;
    int playing;
} Game;

// Function prototypes
void initPlayer(Player *player, float exp_rate);
int startingPlayer();
void reset(Player player[2], int board[3][3]);
int availPos(int board[3][3], Coord availCoord[9]);
void updateBoardState(int board[3][3], Coord action, Game *game);
int check_win(int board[3][3], Game *game);
void saveQTable(Qvalue *q_table[QTABLE_LENGTH], const char *filename);
void loadQTable(Qvalue *q_table[QTABLE_LENGTH], const char *filename);
void train(int episode);
void pve();


#endif