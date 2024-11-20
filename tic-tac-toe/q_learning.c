#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "q_learning.h"
// #include "game_logic.h"
// #include "gui.h"
// #include "player_vs_cpu.h"


/***
 * initPlayer() - initialise a player object
 */
void initPlayer(Player *player, float exp_rate){
    DEBUG_PRINT("Initialising Player...\n");

    // Initialise states
    memset(player->state, 0, sizeof(player->state));

    // Allocating memory for Q-table entries
    for(int i = 0; i< QTABLE_LENGTH; i++){
        player->state_val[i] = malloc(sizeof(Qvalue));

        if(!player->state_val[i]){
            fprintf(stderr, "Memory allocation failed for Q-table enter %d\n", i);
            exit(EXIT_FAILURE);
        }

        memset(player->state_val[i]->key, 0, sizeof(player->state_val[i]->key));
        player->state_val[i]->val = 0.0f;
    }

    // Set exploration rate
    player->exp_rate = exp_rate;

    DEBUG_PRINT("Player initialised successfully\n");
}

/***
 * startingPlayer() - determine the starting player randomly
 */
int startingPlayer(){
    return (rand() % 2 == 0) ? 1 : -1;
}

/***
 * reset() - reset player states and game board
 */
void reset(Player players[2], int board[3][3]){
    // Reset player states
    for(int p = 0; p<2; p++){
        memset(players[p].state, 0, sizeof(players[p].state));
    }

    // Reset board
    memset(board, 0, sizeof(int) * 3 * 3);
}

// Convert char 2D array to int 2D array for qlearning to understand
char convertBoard(char board[3][3], int convertedState[3][3]){
    DEBUG_PRINT("Converting Board...\n");
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
    DEBUG_PRINT("Board Converted\n");
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

/***
 * availPos() - find available positions on the game board
 */
int availPos(int board[3][3], Coord availCoord[9]){
    int index = 0;

    for(int i = 0; i< 3; i++){
        for(int j=0; j < 3; j++){
            if(board[i][j] == BLANK){
                availCoord[index].row = i;
                availCoord[index].col = j;
                index++;
            }
        }
    }
    return index;
}

/**
 * aiMove() - AI selection on the board
 * 
 */
Coord aiMove(Coord position[], int pos_index, int board[3][3], int playerSym, Player *p){
    float max_val = -1e9;
    Coord best_action = position[0];

    // Exploration
    if((float)rand() / RAND_MAX < p->exp_rate){
        int rand_index = rand() % pos_index;
        return position[rand_index];
    }

    // Exploitation
    for(int i = 0; i< pos_index; i++){
        int nextBoard[3][3];
        memcpy(nextBoard, board, sizeof(nextBoard)); 
        nextBoard[position[i].row][position[i].col] = playerSym;

        // Convert board to 1D array for Q-value lookup
        int board1d[MAX_LENGTH];
        memcpy(board1d, &nextBoard[0][0], sizeof(board1d));

        // Check Q-value of the action
        float q_val = 0.0f;
        for(int j=0; j< QTABLE_LENGTH; j++){
            if(memcmp(board1d, p->state_val[j]->key, MAX_LENGTH * sizeof(int)) == 0){
                q_val = p->state_val[j]->val;
                break;
            }
        }

        // Update best action based on Q-value
        if (q_val > max_val){
            max_val = q_val;
            best_action = position[i];
        }
    }

    return best_action;
}



/**
 * PlayerMove() - Human selection on the board
 * 
 */
Coord PlayerMove(Coord position[], int pos_index, int board[3][3]){
    Coord choosen_position;
    while(1){
        printf("Enter row (0-2): ");
        scanf("%d", &choosen_position.row);
        printf("Enter column (0-2): ");
        scanf("%d", &choosen_position.col);

        for(int i = 0; i< pos_index; i++){
            if(position[i].row == choosen_position.row && position[i].col == choosen_position.col){
                return choosen_position;
            }
        }
        printf("Invalid position. Please try again.\n");
    }
}

/**
 * updateBoardState() - Update board after each move
 * 
 */
void updateBoardState(int board[3][3], Coord action, Game *game){
    board[action.row][action.col] = game->playing;
    game->playing = -game->playing; // Switch player
}

/**
 * check_win() - Check if a player has won the game
 */
int check_win(int board[3][3], Game *game){
    int diag_sum1 = 0, diag_sum2 = 0;

    // Check rows and columns and diagonals
    for (int i=0; i<3; i++){
        int row_sum = 0, col_sum = 0;
        
        for(int j=0; j<3; j++){
            row_sum += board[i][j];
            col_sum += board[j][i];
        }

        if(abs(row_sum) == 3 || abs(col_sum) == 3){
            game->game_status = true;
            return row_sum > 0 || col_sum > 0 ? HUMAN : CPU;
        }

        diag_sum1 += board[i][i];
        diag_sum2 += board[i][2-i];
    }

    if (abs(diag_sum1) == 3 || abs(diag_sum2) == 3){
        game->game_status = true;
        return diag_sum1 > 0 || diag_sum2 > 0? HUMAN : CPU;
    }

    // check for draw
    Coord avail_pos[9];
    if(availPos(board, avail_pos) == 0){
        game->game_status = true;
        return 0; 
    }

    return -99; // Game continues
}

void saveQTable(Qvalue *q_table[QTABLE_LENGTH], const char *filename){
    FILE *file = fopen(filename, "wb");

    if(!file){
        perror("Failed to open file for saving Q-table");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < QTABLE_LENGTH; i++){
        if(q_table[i] != NULL){
            fwrite(q_table[i]->key, sizeof(int), MAX_LENGTH, file);
            fwrite(&(q_table[i]->val), sizeof(float), 1, file);
        } 
    }

    fclose(file);
    DEBUG_PRINT("Q-table saved successfully to %s\n", filename);
}

void loadQTable(Qvalue *q_table[QTABLE_LENGTH], const char *filename){
    FILE *file = fopen(filename, "rb");

    if(!file){
        perror("Failed to open file for loading Q-table");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i< QTABLE_LENGTH; i++){
        q_table[i] =malloc(sizeof(Qvalue));
        
        if(fread(q_table[i]->key, sizeof(int), MAX_LENGTH, file) != MAX_LENGTH || 
        fread(&(q_table[i]->val), sizeof(float), 1, file) != 1){
            free(q_table[i]);
            q_table[i] = NULL;
            break;
        }
    }
    fclose(file);
    DEBUG_PRINT("Q-table loaded successfully from %s\n", filename);
}

void train(int episode){
    int board[3][3];
    Player players[2];

    for(int p = 0; p < 2; p++){
        initPlayer(&players[p], 0.3);
    }

    for(int i = 0; i < episode; i++){
        DEBUG_PRINT("Training Round %d\n", i+1);
        Game game={.game_status = false, .playing=startingPlayer()};
        reset(players, board);

        while(!game.game_status){
            int board1d[9];
            Coord avail_pos[9];
            int pos_index = availPos(board, avail_pos);
            Coord action = avail_pos[rand() % pos_index]; // Player picking a random move
            
            updateBoardState(board, action, &game);

            printConvertedBoard(board);

            int win = check_win(board, &game);

            if(win!=-99) break;
        }
    }
    saveQTable(players[0].state_val, "q_table.bin");
}

void pve(){
    int board[3][3] = {0};
    Player ai;

    initPlayer(&ai, 0.2);
    loadQTable(ai.state_val, "q_table.bin");
    
    Game game={.game_status = false, .playing=startingPlayer()};
    
    while(!game.game_status){
        int board1d[9];
        Coord avail_pos[9];
        int pos_index = availPos(board, avail_pos);

        printConvertedBoard(board);

        if(game.playing == HUMAN){
            printf("Your Turn:\n");
            Coord action = PlayerMove(avail_pos, pos_index, board);
            updateBoardState(board, action, &game);
        } else{
            printf("AI's Turn:\n");
            Coord action = aiMove(avail_pos, pos_index, board, CPU, &ai);
            updateBoardState(board, action, &game);
        }

        int winner = check_win(board, &game);
        if(winner != -99){
            printf(winner == HUMAN ? "You Win!\n" : winner == CPU ? "AI Wins!\n" : "It's a draw...\n");
            break;
        }
    }
}


int main(){
    srand(time(NULL));

    train(1000);
    pve();

    return 0;
}