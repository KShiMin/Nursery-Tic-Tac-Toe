#include <stdio.h>
#include <stdlib.h>

const int human = 1, cpu = -1, blank = 0;

char convertBoard(char board[3][3], int convertedState[3][3]){
    

    for(int i=0; i< 3; i++){
        for(int j=0; j<3; j++){
            switch (board[i][j]){
                case 'X':
                    convertedState[i][j] = cpu;
                    break;
                case 'O':
                    convertedState[i][j] = human;
                    break;
                case '-':
                    convertedState[i][j] = blank;       
                    break;
            }
        }
    }

}

void printConvertedBoard(int convertedState[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", convertedState[i][j]);
        }
        printf("\n");
    }
}

void game(char board[3][3]){

}

int main(){
    char board[3][3] = {
        {'X', 'O', 'X'},
        {'-', 'O', '-'},
        {'-', 'X', 'O'}
    };

    int convertedState[3][3];
    convertBoard(board, convertedState);

    printf("Converted Board:\n");
    printConvertedBoard(convertedState);

    return 0;
}