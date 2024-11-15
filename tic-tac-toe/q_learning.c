#include <stdio.h>
#include <stdlib.h>

// Q-Learning Parameteres
#define NUM_STATES 19683
#define NUM_ACTIONS 9
#define ALPHA 0.1
#define GAMMA 0.9
#define EPSILON 0.6

// Initialize start of Q-table with zeros
double qTable[NUM_STATES][NUM_ACTIONS] = {0};

// Function to convert board state to a unique state ID to store all possible board configuration
int getState(char board[3][3]){
    int state = 0;
    for (int row = 0; row < 3; row ++){
        for (int col=0; col < 3; col++){
            int cell_val;
            if(board[row][col] == '-'){
                cell_val = 0;
            } else if (board[row][col] == 'X'){
                cell_val = 1;
            } else if (board[row][col] == 'O'){
                cell_val = 2;
            }
            state = state * 3 + cell_val; // update possible board statues with unique calculation
            printf("Q-table[%d][%d] = %.6lf ", row, col, qTable[row][col]);
        }
    printf("\n");
    }
    return state;
}

int selectAIMove(int state){
    int random_val = rand();
    if ((double) random_val/ RAND_MAX < EPSILON){
        // Allow q-learning to explore by selecting a random action
        return random_val % NUM_ACTIONS;
    } else{
        // Allow q-learning to exploit by selecting know action with the highest Q-value
        int best_action = 0;
        double max_q = qTable[state][0];
        for(int a=1; a < NUM_ACTIONS; a ++){
            if (qTable[state][a] > max_q){
                max_q = qTable[state][a];
                best_action = a;
            }
        }
        return best_action;
    }
}


void updateQ(int state, int action, int reward, int next_state) {
    // Ensure that the states are within bounds
    if (state >= NUM_STATES || next_state >= NUM_STATES) {
        printf("Error: State or next_state out of bounds: state=%d, next_state=%d\n", state, next_state);
        return;
    }

    double max_q_next = qTable[next_state][0];
    for (int a = 1; a < NUM_ACTIONS; a++) {
        if (qTable[next_state][a] > max_q_next) {
            max_q_next = qTable[next_state][a];
        }
    }
    double old_q = qTable[state][action];
    qTable[state][action] += ALPHA * (reward + GAMMA * max_q_next - qTable[state][action]);

    // Print for debugging to verify Q-table updates
    printf("\nUpdating Q[%d][%d]: Old Q = %.6lf, Reward = %d, New Q = %.6lf\n",
           state, action, old_q, reward, qTable[state][action]);
}


int check_endgame(char board[3][3], int *reward){
    // Checks rows and column for any wins
    for (int i=0; i<3; i++){
        if(board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != '-'){
            *reward = (board[i][0] == 'X') ? 1 : -1;
            printf("Across a Row! reward is %d\n", *reward);
            return *reward;
        }
        if(board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != '-'){
            *reward = (board[0][i] == 'X') ? 1 : -1;
            printf("Across a Column! reward is %d\n", *reward);
            return *reward;
        }
    }

    // Check diagonals for a win
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != '-') {
        *reward = (board[0][0] == 'X') ? 1 : -1;  // +1 for X win, -1 for O win
        printf("Across a Diagonals! reward is %d\n", *reward);
        return *reward;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != '-') {
        *reward = (board[0][2] == 'X') ? 1 : -1;  // +1 for X win, -1 for O win
        printf("Across a Diagonals! reward is %d\n", *reward);
        return *reward;
    }

    // Check for draw (no empty spaces)
    int is_draw = 1;  // Assume the board is full
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == '-') {
                is_draw = 0;  // Found an empty space
                break;
            }
        }
        if (!is_draw) break;
    }

    if (is_draw) {
        *reward = 0;  // Neutral reward for a draw
        return 0;     // Return 0 for a draw
    }

    *reward = 0;     // No reward for an ongoing game
    return 2;        // Return 2 if the game is ongoing
}

void print_board(char board[3][3])
{
    printf("\n\n");
    printf("%c | %c | %c\n", board[0][0], board[0][1], board[0][2]);
    printf("%c | %c | %c\n", board[1][0], board[1][1], board[1][2]);
    printf("%c | %c | %c\n", board[2][0], board[2][1], board[2][2]);
    printf("\n\n");
}

void update_board(char board[3][3], int row, int col, char curr_player)
{
    if (board[row][col] == '-')
    {
        board[row][col] = curr_player;
    }
}

void makeMove(char board[3][3], int action, char player) {
    printf("At makeMove, action is %d\n", action);
    int row = action / 3;
    int col = action % 3;
    board[row][col] = player;
    update_board(board, row, col, player);
    print_board(board);
}

void train(){
    printf("Training Started...\n");
    for (int eps = 0; eps < 1000; eps ++){
        char board[3][3] = { {'-', '-', '-'}, {'-', '-', '-'}, {'-', '-', '-'} };
        int state = getState(board);
        int reward = 0;
        int done = 0;
        char player = 'X';  // Start with player X (AI)

        print_board(board);

        while(!done){
            int act, row, col;
            do{
                act = selectAIMove(state);
                printf("action is %d\n", act);
                row = act/3;
                col = act%3;
            } while (board[row][col] != '-');

            makeMove(board, act, player);

            int game_state = check_endgame(board, &reward);
            printf("game state is %d\n", game_state);

            if (game_state !=2){
                done = 1;
            }else{
                player = (player == 'X') ? 'O': 'X';
            }

            int next_state = getState(board);

            if (player == 'X'){
                printf("Updating Q-table...\n");
                updateQ(state, act, game_state, next_state);
            }

            state = next_state;
        }
    }
}

int main(){
    train();
    printf("Training completed!");
    return 0;
}