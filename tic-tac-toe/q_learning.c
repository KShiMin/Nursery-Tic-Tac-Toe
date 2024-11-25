#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "q_learning.h"

const float LR = 0.2f;      // Learning rate for Q-value updates
const float DECAY = 0.9f;   // Decay factor for exploration rate over episodes

/***
 * initPlayer(): Initialize a Player
 * 
 * Allocates memory for the Q-table, sets the player's state to default, 
 * and assigns an exploration rate
 * 
 * params:
 *  - Player *player: pointer to the Player object to initialize
 *  - float exp_rate: exploration rate for the AI
 */
void initPlayer(Player *player, float exp_rate){
    DEBUG_PRINT("Initialising Player...\n");

    // Initialise player's sate array to zero
    memset(player->state, 0, sizeof(player->state));

    // Allocate memory for Q-table entries and initialise values
    for(int i = 0; i< QTABLE_LENGTH; i++){
        player->state_val[i] = malloc(sizeof(Qvalue));  // Allocate memory for a Qvalue struct

        // Check if memory allocation was successful
        if(!player->state_val[i]){
            fprintf(stderr, "Memory allocation failed for Q-table enter %d\n", i);
            exit(EXIT_FAILURE); // Exit the program is memory allocation fails
        }

        // Initialise the key and value of Qvalue struct
        memset(player->state_val[i]->key, 0, sizeof(player->state_val[i]->key));    // Set the key array to zero
        player->state_val[i]->val = 0.0f;   // Set the Q-value to 0.0
    }

    // Set player's exploration rate to the provided value
    player->exp_rate = exp_rate;

    DEBUG_PRINT("Player initialised successfully\n");
}

/***
 * startingPlayer(): Determine the starting player
 * 
 * Randomly selects HUMAN (1) or CPU (-1) as the starting player
 *
 * return:
 *  - int: 1 for HUMAN, -1 for CPU
 */
int startingPlayer(){
    // Generate a random number and return 1 for HUMAN or -1 for CPU
    return (rand() % 2 == 0) ? 1 : -1;
}

/***
 * reset(): Reset player states and game board
 * 
 * Clears player states and resets the game board for a new round
 * 
 * params:
 *  - Player players[2]: array of Player objects
 *  - int board[3][3]: game board to reset
 */
void reset(Player players[2], int board[3][3]){
    // Loop through each player to reset their state
    for(int p = 0; p<2; p++){
        memset(players[p].state, 0, sizeof(players[p].state));  // Set player state to zero
    }

    // Clear the game board by setting all cells to 0 (empty)
    memset(board, 0, sizeof(int) * 3 * 3);
}


/***
 * convertBoard(): Convert character board to integer board
 * 
 * Converts the character-based game board (e.g., 'X', 'O', '-') to an 
 * integer-based board (CPU, HUMAN, BOARD_BLANK) for the AI to process and learn.
 * 
 * params:
 *  - char board[3][3]: character board to convert
 *  - int convertedState[3][3]: integer board to store converted values
 */
char convertBoard(char board[3][3], int convertedState[3][3]){
    DEBUG_PRINT("Converting Board...\n");
    
    // Loop through each cell in the character board
    for(int i=0; i< 3; i++){
        for(int j=0; j<3; j++){
            // Map characters to their integer equivalents
            switch (board[i][j]){
                case 'X':   // If cell contains 'X', set it to CPU's symbol
                    convertedState[i][j] = CPU;
                    break;
                case 'O':   // If cell contains 'O', set it to HUMAN's symbol
                    convertedState[i][j] = HUMAN;
                    break;
                case '-':   // If cell contains '-', set it to BOARD_BLANK symbol
                    convertedState[i][j] = BOARD_BLANK;       
                    break;
            }
        }
    }
    DEBUG_PRINT("Board Converted\n");
}


/***
 * printConvertedBoard(): Display board
 * 
 * Displays the current state of the integer-based game board to the console.
 * 
 * params:
 *  - int convertedState[3][3]: integer board to print
 */
void printConvertedBoard(int convertedState[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", convertedState[i][j]);   // Print value of each cell
        }
        printf("\n");
    }
    printf("\n");
}

/***
 * availPos(): Get available positions
 * 
 * Finds empty spots on the board and stores their coordinates.
 * 
 * params:
 *  - int board[3][3]: current game board
 *  - Coord availCoord[9]: array to store available positions
 * 
 * return:
 *  - int: number of available positions
 *
 */
int availPos(int board[3][3], Coord availCoord[9]){
    int index = 0;  // Initialise index for available positions

    // Loop through the board and find empty cells
    for(int i = 0; i< 3; i++){
        for(int j=0; j < 3; j++){
            if(board[i][j] == BOARD_BLANK){ // Check if cell is empty
                availCoord[index].row = i;  // Store row of empty cell
                availCoord[index].col = j;  // Store column of empty cell
                index++;    // Increment index for next position
            }
        }
    }
    return index;   // Return number of available positions
}

void defaultQValue(Qvalue *q_table[QTABLE_LENGTH], int state[]) {
    for (int i = 0; i < QTABLE_LENGTH; i++) {
        if (q_table[i]->key[0] == 0) { // Check if the key is empty (first element is 0)
            memcpy(q_table[i]->key, state, MAX_LENGTH * sizeof(int)); // Copy the state
            q_table[i]->val = 0.0f; // Initialize the Q-value as 0
            DEBUG_PRINT("Default value set for Q-value at index %d\n", i);
            return;
        }
    }
    fprintf(stderr, "Error: Q-table is full, cannot add new state.\n");
    exit(EXIT_FAILURE); // Ensure the program exits gracefully when Q-table is full
}


// find the index of the state in the q table
int findQValue(int state[MAX_LENGTH], Qvalue *q_table[QTABLE_LENGTH]) {
    for (int i = 0; i < QTABLE_LENGTH; i++) {
        if (q_table[i]->key[0] == 0) break; // Stop searching when reaching an empty slot
        if (memcmp(q_table[i]->key, state, MAX_LENGTH * sizeof(int)) == 0) {
            return i; // State found
        }
    }
    return -1; // State not found
}

// Add state to current player
void addState(Player *p, int board1d[MAX_LENGTH]) {
    for (int i = 0; i < MAX_STRINGS; i++) {
        if (p->state[i][0] == 0) { // Find the first empty slot
            memcpy(p->state[i], board1d, MAX_LENGTH * sizeof(int)); // Copy the state
            DEBUG_PRINT("State added at index %d\n", i);
            return;
        }
    }
    fprintf(stderr, "Error: Player state array is full, cannot add new state.\n");
    exit(EXIT_FAILURE); // Ensure the program exits gracefully when the state array is full
}


// Update Q-table with rewards
void updateQtable(Player *player, int winner) {
    float reward = (winner == 1) ? 0.0f : (winner == -1) ? 1.0f : 0.5f; // Define the reward
    DEBUG_PRINT("Updating Q-table with reward %.2f\n", reward);

    for (int i = MAX_STRINGS - 1; i >= 0; i--) {
        if (player->state[i][0] == 0) continue; // Skip uninitialized states

        int q_index = findQValue(player->state[i], player->state_val);
        if (q_index == -1) {
            defaultQValue(player->state_val, player->state[i]); // Add new state to Q-table
            q_index = findQValue(player->state[i], player->state_val);
        }

        // Compute the maximum Q-value for the next state
        float max_next_q = 0.0f;
        if (i + 1 < MAX_STRINGS && player->state[i + 1][0] != 0) {
            int next_q_index = findQValue(player->state[i + 1], player->state_val);
            if (next_q_index != -1) {
                max_next_q = player->state_val[next_q_index]->val;
            }
        }

        // Apply the Q-learning formula
        player->state_val[q_index]->val += LR * (reward + DECAY * max_next_q - player->state_val[q_index]->val);

        DEBUG_PRINT("Updated Q-value at index %d: %.2f\n", q_index, player->state_val[q_index]->val);
        reward *= DECAY; // Propagate reward backward through visited states
    }
    DEBUG_PRINT("Q-table updated successfully.\n");
}


/***
 * aiMove(): Select AI's move on the board
 * 
 * Chooses the best move based on exploration or exploitation (Q-table values).
 * 
 * params:
 *  - Coord position[]: array of available positions
 *  - int pos_index: number of available positions
 *  - int board[3][3]: current game board
 *  - int playerSym: symbol representing the AI player
 *  - Player *p: pointer to the AI Player object
 * 
 * return:
 *  - Coord: AI's chosen position
 */
Coord aiMove(Coord position[], int pos_index, int board[3][3], int playerSym, Player *p){
    float max_val = -1e9;   // Initialise max_val to a very small number
    Coord best_action = position[0];    // Default to first available position

    // Exploration
    if((float)rand() / RAND_MAX < p->exp_rate){
        int rand_index = rand() % pos_index;    // Pick a ranom position
        return position[rand_index];
    }

    // Exploitation
    for(int i = 0; i< pos_index; i++){
        int nextBoard[3][3];

        memcpy(nextBoard, board, sizeof(nextBoard));    // Copy current board

        nextBoard[position[i].row][position[i].col] = playerSym;    // Simulate move

        int board1d[MAX_LENGTH];
        memcpy(board1d, &nextBoard[0][0], sizeof(board1d)); // Convert board to 1D array

        // Search Q-value for the a given state
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


/***
 * PlayerMove(): Get human player's move
 * 
 * Prompts the human player to enter their move and validates the input 
 * against the available positions on the board.
 * 
 * params:
 *  - Coord position[]: array of available positions
 *  - int pos_index: number of available positions
 *  - int board[3][3]: current game board
 * 
 * return:
 *  - Coord: player's chosen position
 */
Coord PlayerMove(Coord position[], int pos_index, int board[3][3]){
    Coord choosen_position; // Store player's choosen position
    
    // Loop till a valid position is choosen
    while(1){
        // Prompt player to enter coordinates of their desired grid
        printf("Enter row (0-2): ");
        scanf("%d", &choosen_position.row);
        printf("Enter column (0-2): ");
        scanf("%d", &choosen_position.col);

        // Check if the entered position is valid
        for(int i = 0; i< pos_index; i++){
            if(position[i].row == choosen_position.row && position[i].col == choosen_position.col){
                return choosen_position;
            }
        }
        printf("Invalid position. Please try again.\n");
    }
}

/***
 * updateBoardState(): Update game board and switch player
 * 
 * Updates the game board and alternates the turn
 * 
 * params:
 *  - int board[3][3]: current game board
 *  - Coord action: position of the player's action
 *  - Game *game: pointer to the Game object
 */
void updateBoardState(int board[3][3], Coord action, Game *game){
    board[action.row][action.col] = game->playing;  // Update the board with current player's symbol
    game->playing = -game->playing; // Switch player
}

/***
 * check_win(): Check game status
 * 
 * Determines if there's a winner, draw, or if the game continues
 * 
 * params:
 *  - int board[3][3]: current game board
 *  - Game *game: pointer to the Game object
 * 
 * return:
 *  - int: HUMAN for human win, CPU for AI win, 0 for draw, -99 for ongoing game
 */
int check_win(int board[3][3], Game *game){
    int diag_sum1 = 0, diag_sum2 = 0;

    // Check rows, columns and diagonals
    for (int i=0; i<3; i++){
        int row_sum = 0, col_sum = 0;   // Sums for current row and column
        
        for(int j=0; j<3; j++){
            row_sum += board[i][j]; // Sum the row
            col_sum += board[j][i]; // Sum the column
        }

        // Check if any row or column is a win
        if(abs(row_sum) == 3 || abs(col_sum) == 3){
            game->game_status = true;   // Mark the game as finished
            return row_sum > 0 || col_sum > 0 ? HUMAN : CPU;
        }

        diag_sum1 += board[i][i];   // Sum for the first diagonal (Top Left to Bottom Right)
        diag_sum2 += board[i][2-i]; // Sum for the second diagonal (Top Right to Bootom Left)
    }

    // Check if any diagonal is a win
    if (abs(diag_sum1) == 3 || abs(diag_sum2) == 3){
        game->game_status = true;   // Mark the game as finished
        return diag_sum1 > 0 || diag_sum2 > 0? HUMAN : CPU; // Return winner
    }

    // check for draw
    Coord avail_pos[9];
    if(availPos(board, avail_pos) == 0){
        game->game_status = true;   // Mark the game as finished
        return 0;   
    }

    return -99; // Game continues
}

/***
 * saveQTable(): Save Q-table to a file
 * 
 * Serializes and saves the Q-table to a binary file for future use.
 * 
 * params:
 *  - Qvalue *q_table[QTABLE_LENGTH]: Q-table to save
 *  - const char *filename: binary filename to save the Q-table
 */
void saveQTable(Qvalue *q_table[QTABLE_LENGTH], const char *filename){
    FILE *file = fopen(filename, "wb"); // Open file in write-binary mode

    // Check if file is successfully opened
    if(!file){
        perror("Failed to open file for saving Q-table");
        exit(EXIT_FAILURE);
    }

    // Write each Q-values to the file
    for(int i = 0; i < QTABLE_LENGTH; i++){
        if(q_table[i] != NULL){
            fwrite(q_table[i]->key, sizeof(int), MAX_LENGTH, file); // Write the key
            fwrite(&(q_table[i]->val), sizeof(float), 1, file);     // Write the value
        } 
    }

    fclose(file);   // Close the file after writing
    DEBUG_PRINT("Q-table saved successfully to %s\n", filename);
}


/***
 * loadQTable(): Load Q-table from a file
 * 
 * Reads a previously saved Q-table from a binary file and loads it into memory.
 * 
 * params:
 *  - Qvalue *q_table[QTABLE_LENGTH]: array to store loaded Q-table entries
 *  - const char *filename: binary filename of the saved Q-table
 */
void loadQTable(Qvalue *q_table[QTABLE_LENGTH], const char *filename){
    FILE *file = fopen(filename, "rb"); // open file in read-binary mode

    // Check if file is successfully opened
    if(!file){
        perror("Failed to open file for loading Q-table");
        exit(EXIT_FAILURE);
    }

    // Read each Q-values from the file
    for(int i=0; i< QTABLE_LENGTH; i++){
        q_table[i] =malloc(sizeof(Qvalue)); // Allocate memory based off Qvalue struct size
        
        // Attempt to read key and value from file
        if(fread(q_table[i]->key, sizeof(int), MAX_LENGTH, file) != MAX_LENGTH || 
        fread(&(q_table[i]->val), sizeof(float), 1, file) != 1){
            free(q_table[i]);   // Free memory if read fails
            q_table[i] = NULL;  // set pointer to NULL
            break;
        }
    }
    fclose(file);   // Close the file after reading
    DEBUG_PRINT("Q-table loaded successfully from %s\n", filename);
}


/***
 * trainModel(): Train AI model
 * 
 * Trains the AI by letting it play against itself for a specified number of rounds.
 * Saves the trained Q-table to a file.
 * 
 * params:
 *  - int episode: number of rounds user want AI to play against itself
 *  - int board[3][3]: converted intger 2D character board array
 */
void trainModel(int episode, int board[3][3]){
    // Initialise a array of size 2
    Player players[2];
    int win1, win2;

    // Initialise Players
    for(int p = 0; p < 2; p++){
        initPlayer(&players[p], 0.3f);
    }

    // Start AI training
    for(int i = 0; i < episode; i++){
        DEBUG_PRINT("Training Round %d\n", i+1);
        // Initialise game variables and randomly choose a starting player
        Game game={.game_status = false, .playing=startingPlayer()};
        
        reset(players, board); // reset player and board after each round

        // Start the game loop until game ends
        while(!game.game_status){
            for (int p = 0; p < 2; p++) {
                Coord avail_pos[9];
                int pos_index = availPos(board, avail_pos);
                Coord action = avail_pos[rand() % pos_index]; // Player pick a random move
                
                updateBoardState(board, action, &game); // Update board

                int board1d[MAX_LENGTH];
                memcpy(board1d, &board, sizeof(board1d)); // Convert board to 1D array
                addState(&players[p], board1d);

                // Get game status
                int win = check_win(board, &game);

                // Check if game contiunes (no winner / draw)
                if(win!=-99) {
                    if (game.playing == HUMAN){
                        win1 += 1;
                    } else if (game.playing == CPU){
                        win2 += 1;
                    }
                    updateQtable(&players[p], win); // If got winner, update Q-table
                    break;
                }
            }
        }
    }

    printf("Total Game Player 1 Won = %d\n", win1);
    printf("Total Game Player 2 Won = %d\n", win2);
    
    // Save trained Q-table to file for future use
    saveQTable(players[0].state_val, "q_table.bin");
}


/***
 * pve(): Player vs AI game
 * 
 * A console-based game mode where the player competes against the AI.
 * 
 * params:
 *  - int board[3][3]: converted intger 2D character board array
 */
void pve(int board[3][3]){
    // int board[3][3] = {0};
    Player ai;

    initPlayer(&ai, 0.2f);   // Initialise AI with exploration rate
    
    // Load trained Q-table from file and store to AI state_val
    loadQTable(ai.state_val, "q_table.bin"); 

    // Initialise game variables and randomly choose a starting player
    Game game={.game_status = false, .playing=startingPlayer()};
    DEBUG_PRINT("Starting game, initial player: %s\n", game.playing == HUMAN ? "HUMAN" : "CPU");

    // Start the game loop until game ends
    while(!game.game_status){
        Coord avail_pos[9];
        int pos_index = availPos(board, avail_pos); // Get available positions

        // Print the current state of the board
        printConvertedBoard(board);

        // Check if it's human's turn or AI's turn, and get the move from player or AI respectively
        if(game.playing == HUMAN){
            printf("Your Turn:\n");
            Coord action = PlayerMove(avail_pos, pos_index, board); // get Player move
            updateBoardState(board, action, &game); // Update board
        } else{
            DEBUG_PRINT("AI is deciding its move...\n");
            Coord action = aiMove(avail_pos, pos_index, board, CPU, &ai); // get AI move
            updateBoardState(board, action, &game); // Update board
        }

        // Check if the game has ended, print the result and break the loop
        int winner = check_win(board, &game);
        if(winner != -99){
            printf(winner == HUMAN ? "You Win!\n" : winner == CPU ? "AI Wins!\n" : "It's a draw...\n");
            break;
        }
    }
}

/***
 * guiMLmove(): Handles AI move with optional training
 * 
 * Handles AI move in GUI mode and optionally trains the AI model
 * 
 * params:
 *  - int training: if model training is required
 *  - char board[3][3]: 2D character board arrray to be converted
 * 
 * return:
 *  - action: contains AI selection in row and column (Coordinate)
 */
Coord guiMLmove(char board[3][3]){
    
    int intBoard[3][3];

    srand(time(NULL));      // Seed random number generator for training.

    // Convert the character board to integer board for the AI to work with
    convertBoard(board, intBoard);
    
    Player ai;

    initPlayer(&ai, 0.2);   // Initialize AI with exploration rate
    
    // Load trained Q-table from file and store to AI state_val
    loadQTable(ai.state_val, "q_table.bin"); 
    
    Coord avail_pos[9];
    int pos_index = availPos(intBoard, avail_pos); // Get available positions

    // AI decides its next move
    Coord action = aiMove(avail_pos, pos_index, intBoard, CPU, &ai);

    return action; 

}

// Uncomment this to train model or play in console/terminal
// int main(){
//     int board[3][3] = {0};
//     trainModel(500, board);
//     int board2[3][3] = {0};
//     pve(board2);
// }