/* Define preprocessor statements */

/* Define include guards. Ensures contents of this file does not get included more than once */
#ifndef GUI_H       /* Run the following if GUI_H has not been defined */
#define GUI_H       /* Defines GUI_H */

#include "raylib.h"         /* Include Raylib GUI external library */
#include "game_logic.h"     /* Include game_logic header file */
#include "minimax.h"
#include "q_learning.h"     // Include Q learning header file
#include <time.h>           // For seed randoming & time calculation

/* Initialise constants used */
#define SCREEN_WIDTH 1000       /* Initialise GUI window width */
#define SCREEN_HEIGHT 1000      /* Initialise GUI window height */
#define GRID_OFFSET 150         /* Initialise an spacing offset from the edges of the GUI for the game grid */
#define PVP 1                   /* Initialise game mode 1 as Player vs Player */         
#define PVC 2                   /* Initialise game mode 2 as Player vs Computer (Minimax algo) */ 
#define PVML 3                  /* Initialise game mode 3 as Player vs Computer (Machine learning algo) */
#define STATE_MENU 0            /* Initialise game state 0 as main menu  */
#define STATE_PLAYING 1         /* Initialise game state 1 as ongoing game */
#define STATE_WIN 2             /* Initialise game state 2 as game end with winner */
#define STATE_DRAW 3            /* Initialise game state 2 as game end with no winner */
#define X_PLAYER 'X'            /* Initialise X player */
#define O_PLAYER 'O'            /* Initialise O player */
extern int gameState;

/* Initialise functions in gui.c */
void draw_menu();       
void game_start();
void draw_grid();
int getBoundary();
int getCellSize();
void CheckMouseInput();
void draw_markers();
void draw_o(int row, int col);
void draw_x(int row, int col);
void game_over();
void restartButton();
void displayScoreBoard();
void displayCurrentPlayer();
void downDifficulty();
void upDifficulty();
void avgCalc(char *algo);

#endif
