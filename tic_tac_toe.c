/* Define preprocessor statements */
#include <stdio.h>
#include "player_vs_player.h"
#include "player_vs_cpu.h"

/* Initialise global variables */
void main_menu();


/* Main program */
int main()
{
    main_menu();  /* Calls main_menu function */

    return 0;  /* Exit program with 0 errors */
}


/********************************************************
function: main_menu
    shows user the main menu in console, and selects game
    mode based on user input
********************************************************/
void main_menu()
{
    /* Iniialise function variables */
    int game_mode;  /* Stores the tic tac toe game mode*/

    printf("\n Tic Tac Toe");
    printf("\n Welcome to Tic Tac Toe! Please select your game mode:\n");
    printf("\n 1. Player Vs Player\n 2. Player Vs CPU\n\n"); /* Shows user game mode choices */
    scanf("%i", &game_mode);  /* Gets user input of choice of game mode and stores it in variable game_mode */

    switch (game_mode)
    {
    case 1:
        playPlayer();
    
    case 2:
        playCPU();
    }

}