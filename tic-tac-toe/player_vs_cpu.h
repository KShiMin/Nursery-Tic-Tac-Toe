/* Define include guards. Ensures contents of this file does not get included more than once*/
#ifndef PLAYER_VS_CPU_H  /* Run the following if PLAYER_VS_CPU_H has not been defined */
#define PLAYER_VS_CPU_H  /* Defines PLAYER_VS_PLAYER */

// Q-Learning Parameteres
#define NUM_STATES 19683
#define NUM_ACTIONS 9
#define ALPHA 0.1
#define GAMMA 0.9
#define EPSILON 0.6

/* Declare global variables of player_vs_cpu.c file*/
void playCPU();


#endif  /* End of header file */
