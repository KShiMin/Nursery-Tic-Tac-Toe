/* Define preprocessor statements */

#include "gui.h"    /* Include gui header file */

/* Main program - program starts here */
int main()
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tic Tac Toe");     /* Initialise GUI window */

    SetTargetFPS(15);                                           /* Set GUI target FPS*/

    while (!WindowShouldClose())                                /* Run the code below while user has not requested to close window */
        {
            BeginDrawing();                 /* Start drawing in GUI */
            ClearBackground(DARKBLUE);      /* Clear the GUI with dark blue */
            game_start();                   /* Call game_start() function from gui.c */
            EndDrawing();                   /* End drawing in GUI */
        }
    CloseWindow();                                              /* Close window */

    return 0;                                                   /* Exit program, program has executed successfully. */
}

