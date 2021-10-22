#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "snake.h"

void clear(){
    #if defined(__linux__) || defined(__unix__) || defined(__Apple__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}

int main(int argc, char *argv[]){
    srand(time(NULL));
    int xsize = strtol(argv[1], NULL, 10);
    int ysize = strtol(argv[2], NULL, 10);
    enum Direction dir = Right;

    Board* board = create_board(create_snake(xsize, ysize), NULL, xsize, ysize);
    int i;
    for (i = 0; i < 5; i++) {
        add_new_food(board);
    }

    while(true) {
        clear();
        for (int i = 0; i < board->ysize; i++)
        {
            for (int j = 0; j < board->xsize; j++)
            {            
                switch (snake_or_food(board, j, i))
                {
                case 1:
                    printf("@");
                    break;
                case 2:
                    printf("o");
                    break;
                case 3:
                    printf("X");
                    break;
                default:
                    printf(".");
                    break;
                }
            }
            printf("\n");
        }

        enum Status status = move_snake(board, dir);
        if (status == Failure) break;
        PointList* snake = board->snake;
        int count_snake = 0;
        while (snake)
        {
            count_snake++;
            snake = snake->next;
        }
        printf("%d\n", count_snake);        
        waitFor(1);
        }
    return 0;
}