/* Define include guards. Ensures contents of this file does not get included more than once*/
#ifndef GUI_H  /* Run the following if GUI_H has not been defined */
#define GUI_H  /* Defines GUI_H */

#include "raylib.h"
// #include "game_logic.h"

#define SCREEN_WIDTH 1000       /* GUI window width */
#define SCREEN_HEIGHT 1000      /* GUI window height */
#define GRID_OFFSET 150
#define BUTTON_OFFSET 10
#define PVP 1                   /* Define game mode 1 as Player vs Player */         
#define PVC 2                   /* Define game mode 2 as Player vs Computer */ 
#define STATE_MENU 0
#define STATE_PLAYING 1
#define STATE_WIN 2
#define STATE_DRAW 3
#define X_PLAYER 'X'
#define O_PLAYER 'O'
#define PERFECT 1
#define IMPERFECT 0
extern int gameState;

// #define WIDTH_OFFSET 50

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


#endif




// void draw_boundary()
// {
//     DrawLine(0, HEIGHT_OFFSET, SCREEN_WIDTH, HEIGHT_OFFSET, WHITE);
//     DrawLine(WIDTH_OFFSET, 0, WIDTH_OFFSET, SCREEN_HEIGHT, WHITE);
//     DrawLine(0, SCREEN_HEIGHT - HEIGHT_OFFSET, SCREEN_WIDTH, SCREEN_HEIGHT - HEIGHT_OFFSET, WHITE);
//     DrawLine(SCREEN_WIDTH - WIDTH_OFFSET, 0, SCREEN_WIDTH - WIDTH_OFFSET, SCREEN_HEIGHT, WHITE);
// }