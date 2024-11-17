#include <stdio.h>
#include <stdlib.h>

#define MAX_STRINGS 100
#define MAX_LENGTH 100


const int human = 1, cpu = -1, blank = 0;

typedef enum{
    false = 0,
    true = 1
} Bool;

typedef struct{
    int row, col;
} Coord;

// Determine q-value of each state
// key = 1D array of state (as a string format)
// val = actual q-value of state
typedef struct{
    char key[9];
    float val;

} Qvalue;

// Simulate AI
typedef struct{
    
    char state[MAX_STRINGS][MAX_LENGTH]; // hold all state movements
    Qvalue *state_val[1000]; // Array of q-values (Q-Table)
    float lr, exp_rate, decay; //Q-learning parameters

} Player;

// Defines variables for each game
typedef struct{
    Bool game_status;
    Player p1, p2; // p1 and p2 represents AI for training
    int playing; // starting player
} Game;

// Convert char 2D array to int 2D array for qlearning to understand
char convertBoard(char board[3][3], int convertedState[3][3]){
    
    for(int i=0; i< 3; i++){
        for(int j=0; j<3; j++){
            switch (board[i][j]){
                case 'X':
                    convertedState[i][j] = cpu;
                    break;
                case 'O':
                    convertedState[i][j] = human;
                    break;
                case '-':
                    convertedState[i][j] = blank;       
                    break;
            }
        }
    }

}

void printConvertedBoard(int convertedState[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", convertedState[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// converts 2d array to 1d array
void boardToString(int board[3][3], int results[9]){
    int index = 0;

    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
            results[index] = board[i][j];
            index++;
        }
    }
}

// return mem add of created array
int availPos(int board[3][3], Coord availCoord[9]){
    
    int index = 0;

    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
            if (board[i][j] == 0){
                availCoord[index].row = i;
                availCoord[index].col = j;
                index++;
            }
        }
    }
    return index;
}

// paras: board --> convertedBoard
void train(int board[3][3]){
    int trainBoard[3][3] = {0};

}

int main(){
    char board[3][3] = {
        {'X', 'O', 'X'},
        {'-', 'O', '-'},
        {'-', 'X', 'O'}
    };

    int convertedState[3][3];
    convertBoard(board, convertedState);

    printf("Converted Board:\n");
    printConvertedBoard(convertedState);

    // int results[9];
    // boardToString(convertedState, results);

    // for(int i=0; i<9; i++){
    //     printf("%d ", results[i]);
    // }

    Coord test[9];

    int test_index = availPos(convertedState, test);
    for (int i=0; i< test_index; i++){
        printf("Coordinate %d: (%d, %d)\n", i, test[i].row, test[i].col);
    }

    return 0;
}