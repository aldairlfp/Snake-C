#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
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

int* unique_ways(enum Direction dir) {
    int * ways = (int*)malloc(2 * sizeof(int*));
    if(dir == 0 || dir == 1) {
        int random0 = rand() % 2 + 2;
        int random1 = (random0 + 1) % 2 + 2;
        ways[0] = random0;
        ways[1] = random1;
    }
    else {
        int random0 = rand() % 2;
        int random1 = (random0 + 1) % 2;     
        ways[0] = random0;
        ways[1] = random1;
    }
    return ways;
}

enum Direction if_collide(Board* board, enum Direction dir) {
    if(next_move(board, dir))
        return dir;
    int * ways = unique_ways(dir);
    for (int i = 0; i < 2; i++) {
        // printf("%d\n", *ways + 1);
        PointList* beginning = next_move(board, *(ways + i)); 
        if(beginning) {
            // printf("%d\n", ways[i]);
            return ways[i];
        }
    }
    return dir;
}

enum Direction find_direction(Board* board, enum Direction dir) {
    int dr[4] = { -1, 1, 0, 0};
    int dc[4] = { 0, 0, -1, 1};
    // printf("Actual en find_direction (%d, %d)\n", new_way->row, new_way->col);
    for (int i = 0; i < sizeof(dr)/sizeof(dr[0]); i++)
    {
        int k = 1;
        while (true)
        {
            int new_r = board->snake->row + k * dr[i];
            int new_c = board->snake->col + k * dc[i];
            int is_snake_or_food = snake_or_food(board, new_r, new_c);
            if(is_out_of_board(board, new_r, new_c) || is_snake_or_food == 2) {
                break;
            }
            if(is_snake_or_food == 3) {
                return i;
            }
            k++;
        }        
    }
    return if_collide(board, dir);
}

void print_snake(Board* board) {
    // printf("(%d, %d) \n", board->snake->row, board->snake->col);
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
}

int main(int argc, char *argv[]){
    srand(time(NULL));
    int cols = strtol(argv[1], NULL, 10);
    int rows = strtol(argv[2], NULL, 10);
    enum Direction dir = Right;

    Board* board = create_board(create_snake(rows, cols), NULL, rows, cols);
    // printf("(%d, %d) \n", board->snake->row, board->snake->col);
    int i;
    for (i = 0; i < 5; i++) {
        add_new_food(board);
    }

    enum Status status;

    do {
        clear();
        print_board(board);
        // dir = find_direction(board, next_move(board, dir), dir);
        // printf("%d\n", new_direction);
        dir = find_direction(board, dir);
        sleep(1);
        status = move_snake(board, dir);
    }
    while (status != Failure);
    
    return 0;
}