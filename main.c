#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "snake.h"

// Clear the command line
void clear(){
    #if defined(__linux__) || defined(__unix__) || defined(__Apple__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}

void waitForIt () {
    for (int i = 0; i < 100000000; i++)
    {
        // Wait for it
    }    
}

void print_snake(Board* board) {
    PointList* snake = board->snake;
    int count_snake = 0;
    while (snake)
        {
             printf("(%d, %d) ", snake->row, snake->col);
            count_snake++;
            snake = snake->next;
        }
}


void print_foods(Board* board) {
    PointList* foods = board->foods;
        int count_foods = 0;
        while (foods)
        {
            printf("(%d, %d) ", foods->row, foods->col);
            count_foods++;
            foods = foods->next;
        }
        printf("%d\n", count_foods);
}

void print_board(Board* board) {
    for (int i = 0; i < board->rows; i++)
        {
            for (int j = 0; j < board->cols; j++)
            {            
                switch (snake_or_food(board, i, j))
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
    printf("Score: %d\n", board->score);
}

int main(int argc, char *argv[]){
    srand(time(NULL));
    int cols = strtol(argv[1], NULL, 10);
    int rows = strtol(argv[2], NULL, 10);
    enum Direction dir = Right;

    Board* board = create_board(create_snake(rows, cols), NULL, rows, cols);
    int i;
    for (i = 0; i < 5; i++) {
        add_new_food(board);
    }

    enum Status status;

    do {
        clear();
        print_board(board);
        dir = find_direction(board, dir);
        waitForIt();
        status = move_snake(board, dir);
    }
    while (status != Failure);
    
    return 0;
}