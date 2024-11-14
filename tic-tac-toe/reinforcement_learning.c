#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Q-Learning Parameters Needed
#define NUM_STATES 19683  // 3^9 possible board configurations
#define NUM_ACTIONS 9     // 9 possible moves (grid positions)
#define ALPHA 0.1         // Learning rate
#define GAMMA 0.9         // Discount factor
#define EPSILON 1.0      // Exploration rate

// Initialize Q-table with zeros
double q_table[NUM_STATES][NUM_ACTIONS] = {0};

// Function to convert board state to a unique state ID to store all possible board configuration
int getState(char board[3][3]) {
    int state = 0;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            int cell_value;
            if (board[row][col] == '-') {
                cell_value = 0;
            } else if (board[row][col] == 'X') {
                cell_value = 1;
            } else if (board[row][col] == 'O') {
                cell_value = 2;
            }
            state = state * 3 + cell_value; // Build the state ID in base-3
            printf("Q-table[%d][%d] = %.6lf ", row, col, q_table[row][col]);
        }
        printf("\n");
    }
    return state;
}


int selectAction(int state) {
    // checks if the dividen? of the random number provided is lessen then the epsilon in percentage
    if ((double) rand() / RAND_MAX < EPSILON) { 
        // Explore: select a random action
        printf("\n%lf\n", (double) rand() / RAND_MAX);
        printf("\nusing amp %d\n", (rand() % NUM_ACTIONS));
        return rand() % NUM_ACTIONS;
    } else {
        // Exploit: select the action with the highest Q-value
        int best_action = 0;
        double max_q = q_table[state][0];
        for (int a = 1; a < NUM_ACTIONS; a++) {
            if (q_table[state][a] > max_q) {
                max_q = q_table[state][a];
                best_action = a;
            }
        }
        return best_action;
    }
}

// Update Q-learning 
void updateQ(int state, int action, int reward, int next_state) {
    double max_q_next = q_table[next_state][0];
    for (int a = 1; a < NUM_ACTIONS; a++) {
        if (q_table[next_state][a] > max_q_next) {
            max_q_next = q_table[next_state][a];
        }
    }
    double old_q = q_table[state][action];
    q_table[state][action] += ALPHA * (reward + GAMMA * max_q_next - q_table[state][action]);

    // Print for debugging to verify Q-table updates
    printf("\nUpdating Q[%d][%d]: Old Q = %.6lf, Reward = %d, New Q = %.6lf\n",
           state, action, old_q, reward, q_table[state][action]);
}


int check_game_end(char board[3][3], int *reward) {
    // Check rows and columns for a win
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != '-') {
            *reward = (board[i][0] == 'X') ? 1 : -1;  // +1 for X win, -1 for O win
            return (board[i][0] == 'X') ? 1 : -1;
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != '-') {
            *reward = (board[0][i] == 'X') ? 1 : -1;  // +1 for X win, -1 for O win
            return (board[0][i] == 'X') ? 1 : -1;
        }
    }

    // Check diagonals for a win
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != '-') {
        *reward = (board[0][0] == 'X') ? 1 : -1;  // +1 for X win, -1 for O win
        return (board[0][0] == 'X') ? 1 : -1;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != '-') {
        *reward = (board[0][2] == 'X') ? 1 : -1;  // +1 for X win, -1 for O win
        return (board[0][2] == 'X') ? 1 : -1;
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

void train() {
    printf("Training Started...\n");
    for (int episode = 0; episode < 3; episode++) {
        char board[3][3] = { {'-', '-', '-'}, {'-', '-', '-'}, {'-', '-', '-'} };
        int state = getState(board);
        int reward = 0;
        int done = 0;
        char player = 'X';  // Start with player X (AI)

        print_board(board);

        while (!done) {
            int action;
            int row, col, new_row, new_col;

            // Loop until an empty cell is found
            do {
                action = selectAction(state);
                printf("action is %d", action);
                row = action / 3;
                col = action % 3;
            } while (board[row][col] != '-');  // Repeat if the cell is occupied
            // Make the selected move on the board
            makeMove(board, action, player);
            // Check if the game has ended after the move
            int game_state = check_game_end(board, &reward);
            printf("game state is %d", game_state);

            // Determine if the game is finished
            if (game_state != 2) { // 2 means the game is ongoing
                done = 1;
            } else {
                // If the game continues, switch player
                player = (player == 'X') ? 'O' : 'X';  // Alternate between 'X' and 'O'
            }

            // Get the new state after the move
            int next_state = getState(board);

            // Update the Q-table using the reward and next state (only for the AI's move)
            if (player == 'X') {  // Only update Q-table when it's AI's turn
                updateQ(state, action, reward, next_state);
            }

            // Set the current state to the next state
            state = next_state;
        }
    }
}

int main() {
    train();
    printf("Training completed.\n");
    return 0;
}