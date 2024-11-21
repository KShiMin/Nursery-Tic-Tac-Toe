/* Define preprocessor statements */
#include <stdio.h>
#include "q_learning.h"


int training = 1; // Need to train Q-learning model first
/********************************************************
function: playCPU 

Input:

********************************************************/
void playCPU()
{
    printf("\n\nSelected game mode: Player Vs CPU");
}



void qLearn(){
    printf("\n\nSelected game mode: Q-Learning");
    int board[3][3]; //converted board since player start first
    guiInt(training, board);
    training = 0;
    
    // initialise AI player
    // Player ai;
    
    // initPlayer(&ai, 0.2);
    // loadQTable(ai.state_val, "q_table.bin");

    // pve();
}