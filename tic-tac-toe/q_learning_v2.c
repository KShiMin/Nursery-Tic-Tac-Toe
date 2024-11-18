#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRINGS 100 // num of string array can hold
#define MAX_LENGTH 9 // how long of a string it can hold
#define QTABLE_LENGTH 5000


const float lr=0.2, decay =0.9; //Q-learning parameters


// Player type integer definition
typedef enum {
    BLANK = 0,
    HUMAN = 1,
    CPU = -1
} PlayerType;

// defining own boolean datatye to avoid confusion
typedef enum{
    false = 0,
    true = 1
} Bool;

// defining own coordinates datatype
typedef struct{
    int row, col;
} Coord;

/***
 * Determine q-value of each state
 * params:
 *  - key: holds the 1D array of state as a string format
 *  - val: holds the actual q-value of the state
 */
typedef struct{
    char key[9];
    float val;

} Qvalue;

// Simulate AI
typedef struct{
    
    int state[MAX_STRINGS][MAX_LENGTH]; // hold all state movements
    Qvalue *state_val[QTABLE_LENGTH]; // Array of q-values (Q-Table)
    float exp_rate; 

} Player;

// Defines variables for each game
typedef struct{
    Bool game_status;
    // Player p1, p2; // p1 and p2 represents AI for training
    int playing; // starting player
} Game;

// Initialise Player object
void initPlayer(Player *player, float exp_rate){
    printf("Initialising Player...\n");
    // initalise empty state array
    for (int i=0 ; i < MAX_STRINGS; i++){
        player->state[i][0] = '\0'; // setting states to 0
    }

    // initalise Q-Table with 0
    for(int i=0; i < QTABLE_LENGTH; i++){
        player->state_val[i] = malloc(sizeof(Qvalue)); // allocate memory for each Q-value
    }

    // set exploration rate
    player->exp_rate = exp_rate;
    printf("Player Initialised\n");
}


// Convert char 2D array to int 2D array for qlearning to understand
char convertBoard(char board[3][3], int convertedState[3][3]){
    printf("Converting Board...\n");
    for(int i=0; i< 3; i++){
        for(int j=0; j<3; j++){
            switch (board[i][j]){
                case 'X':
                    convertedState[i][j] = CPU;
                    break;
                case 'O':
                    convertedState[i][j] = HUMAN;
                    break;
                case '-':
                    convertedState[i][j] = BLANK;       
                    break;
            }
        }
    }
    printf("Board Converted\n");
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

    printf("Converting Board to 1d array...\n");

    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
            results[index] = board[i][j];
            index++;
        }
    }
    printf("Board Converted\n");
}

void copyBoard(int board[3][3], int nextBoard[3][3]){
    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
            nextBoard[i][j] = board[i][j];
        }
    }
}

/***
 * function: availPos - creates an array of avaiable position on
 * the tic-tac-tow board for AI to choose their next move
 * 
 * params:
 *  - int board[3][3]: converted tic-tac-toe board
 *  - Coord availCoord[9]: array of avaiable coordinates
 * 
 * return:
 *  - index: number of avaiable position and it's coordinate
 */
int availPos(int board[3][3], Coord availCoord[9]){
    
    int index = 0;

    printf("Getting Available Position...\n");

    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
            if (board[i][j] == 0){
                availCoord[index].row = i;
                availCoord[index].col = j;
                index++;
            }
        }
    }

    printf("Available Position Gotten.\n");

    return index;
}

float getQval(int boardStr[], Player p){
    for(int s=0; s< QTABLE_LENGTH; s++){
        if(memcmp(boardStr, p.state_val[s]->key, 9) == 0){
            return p.state_val[s]->val;
        }
    }
    return 0; // if state is not in q-table, return default value 0
}


// Q-learning algo, playerSym = player symbol
Coord playerAction(Coord position[], int pos_index,int board[3][3], int playerSym, Player p){
    int rand_val = rand();
    int val_max = -999;
    Coord action = position[0];
    
    // Exploration
    if((double) rand_val / RAND_MAX < p.exp_rate){
        int rand_index = rand() % pos_index;
        printf("random index = %d\n", rand_index);
        return position[rand_index];
    }

    for (int i=0; i < pos_index; i++){
        int nextBoard[3][3];
        int board1D[9];
        
        copyBoard(board, nextBoard);
        nextBoard[position[i].row][position[i].col] = playerSym; // place player symbol on respective gride
        

        boardToString(nextBoard, board1D); // convert board to 1D array
        
        // Getting q-value for respective state
        float state_val = getQval(board1D, p);

        if(state_val > val_max){
            val_max = state_val;
            action = position[i];
        }
    }

    return action;

}

// Update Board State
void updateBoardState(int board[3][3], Coord action, Game game){
    printf("Updating Board...\n");
    board[action.row][action.col] = game.playing;
    game.playing = -game.playing; // switch player
    printf("Board Updated\n");
}

// Add state to current player
void addState(Player* p, int board1d[9]){
    for(int i=0; i<MAX_STRINGS; i++){
        if(p->state[i] == '\0'){
            for(int j=0; j<9; j++){
                p->state[i][j] = board1d[j]; //copy 1d array to state
            }
            break;
        }
    }
}


int check_win(int board[3][3], Game* game){
    int diag_sum1 = 0, diag_sum2 = 0;

    // Check rows and columns
    for(int i=0; i<3;i++){
        int row_sum = 0, col_sum = 0;

        for(int j=0; j<3; j++){
            row_sum += board[i][j]; //Sum the row
            col_sum += board[j][i]; //Sum the column
        }

        if (abs(row_sum) == 3){
            game->game_status = true;
            return row_sum > 0 ? 1: -1;
        }

        if (abs(col_sum) == 3){
            game->game_status = true;
            return col_sum > 0? 1: -1;
        }

        // Check Diagonal
        diag_sum1 += board[i][i]; //Top-Left to Bottom-Right
        diag_sum2 += board[i][3-i-1]; // Top-Right to Bottom-Left
    }

    if (abs(diag_sum1) == 3){
        game->game_status = true;
        return diag_sum1 > 0 ? 1: -1;
    }

    if (abs(diag_sum2) == 3){
        game->game_status = true;
        return diag_sum2 > 0 ? 1: -1;
    }

    Coord avail_pos[9];
    int pos_index = availPos(board, avail_pos);
    if(pos_index == 0){
        game->game_status = true;
        return 0; // if no winner, game is tie
    }

    return -99; // Game contiunes
}

// paras: board --> convertedBoard, episode - amt of game rounds
// Training qlearning
void train(int episode){
    
    int board[3][3] = {0};
    Player players[2];

    // Initialise Players
    for(int p=0; p<2; p++){
        initPlayer(&players[p],0.3);
    }

    for (int i=0; i<episode; i++){
        printf("Training Round %d\n", i);
        Game game = {
            game_status: false,
            playing: 1 // players will be represented as 1 or -1
        };

        // 
        while(!game.game_status){   // while game not end
            for(int p=0; p<2; p++){
                int board1d[9];
                Coord avail_pos[9];
                int pos_index = availPos(board, avail_pos);
                Coord action = playerAction(avail_pos, pos_index, board, game.playing, players[p]);
                
                updateBoardState(board, action, game);
                
                boardToString(board, board1d);

                addState(&players[p],board1d);

                int win = check_win(board, &game);
                if(!win == -99){

                };
                
            }
        }   

    }
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
    // for (int i=0; i< test_index; i++){
    //     printf("Coordinate %d: (%d, %d)\n", i, test[i].row, test[i].col);
    // }
    printf("Test Index is %d\n", test_index);

    Player p1;
    initPlayer(&p1, 0.3);

    Coord pos = playerAction(test, test_index,convertedState, -1, p1);
    printf("Row: %d, Column: %d\n", pos.row, pos.col);

    return 0;
}