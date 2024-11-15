#include <stdio.h>
#include <stdlib.h>
#include "game_logic.h"
// #include "gui.h"
#include "player_vs_cpu.h"


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


// Updates Q-Table
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


int assign_reward(int game_state){
    if (game_state == 1){
        return 1; // if AI wins, reward with 1 point
    } else if (game_state == -1){
        return -1; //if AI lose, deduct it's point
    } else if (game_state == 0){
        return 0; // if AI ties, no reward is given
    } else{
        return 0; // Contiune game if no winner or drawn found
    };
}


// Game Logic
int check_endgame(char board[3][3]){
    // Checks rows and column for any wins
    for (int i=0; i<3; i++){
        if(board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != '-'){
            return (board[i][0] == 'X') ? 1 : -1;
        }
        if(board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != '-'){
            return (board[0][i] == 'X') ? 1 : -1;
        }
    }

    // Check diagonals for a win
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != '-') {
        // combination is = {top left, middle, bottom right}
        return (board[0][0] == 'X') ? 1 : -1;  // +1 for X win, -1 for O win
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != '-') {
        // combination is = {top right, middle, bottom left}
        return (board[0][2] == 'X') ? 1 : -1; // +1 for X win, -1 for O win
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
        return 0;     // Return 0 for a draw
    }

    return 2;        // Return 2 if the game is ongoing
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
    for (int eps = 0; eps < 10000; eps ++){
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

            int game_state = check_endgame(board);
            printf("game state is %d\n", game_state);
            int reward = assign_reward(game_state);

            if (game_state !=2){
                done = 1;
            }else{
                player = (player == 'X') ? 'O': 'X';
            }

            int next_state = getState(board);

            if (player == 'X'){
                printf("Updating Q-table...\n");
                updateQ(state, act, reward, next_state);
            }

            state = next_state;
        }
    }
}

void qTableResult(){
    for (int i =0; i< NUM_STATES; i++){
        printf("State %d: ", i);
        for (int action = 0; action < NUM_ACTIONS; action++) {
            printf("%.6lf ", qTable[i][action]);  // Print Q-value with 6 decimal precision
        }
        printf("\n");  // Newline after each state for better readability
    }
}

int main(){
    train();
    printf("Training completed!\n");
    printf("Q-Table after Training\n");
    qTableResult();
    printf("\n");
    return 0;
}