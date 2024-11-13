/* Define preprocessor statements */
#include "gui.h" /* GUI header file */

/* Main program */
int main()
{
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tic Tac Toe");     /* Initialise GUI window */
    
    SetTargetFPS(60);                                           /* Set GUI target FPS*/
    
    while (!WindowShouldClose())        /* Run the code below as long as window is open */
        {
            BeginDrawing();
            ClearBackground(BLACK);
            game_start();
            EndDrawing();
        }
    CloseWindow();

    return 0;  /* Exit program with 0 errors */
}

