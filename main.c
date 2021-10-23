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

enum Direction find_direction(Board* board, PointList* new_way, enum Direction olc_dir) {
    int dr[4] = { -1, 1, 0, 0};
    int dc[4] = { 0, 0, -1, 1};
    printf("Actual en find_direction (%d, %d)\n", new_way->row, new_way->col);

    for (int i = 0; i < sizeof(dr)/sizeof(dr[0]); i++)
    {
        int k = 0;
        // while (true)
        // {
            int new_r = new_way->row + k * dr[i];
            int new_c = new_way->col + k * dc[i];
            
            if(is_out_of_board(board, new_r, new_c)){
                // printf("Me sali\n");
                printf("Nueva (%d, %d)\n", new_r, new_c);
                switch (olc_dir)
                {
                case Up:
                    return Right;
                    break;
                case Down:
                    return Left; 
                    break;
                case Right:
                    return Down; 
                    break;
                default:
                    return Up;
                    break;
                }
                // break;
            }
            // k++;
        // }        
    }
    return olc_dir;
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

    while(true) {
        // clear();
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
        // printf("(%d, %d) \n", board->snake->row, board->snake->col);
        PointList* snake = board->snake;
        int count_snake = 0;
        // while (snake)
        // {
             printf("Actual (%d, %d) \n", snake->row, snake->col);
        //     count_snake++;
        //     snake = snake->next;
        // }
        // PointList* foods = board->foods;
        // int count_foods = 0;
        // while (foods)
        // {
        //     printf("(%d, %d) ", foods->row, foods->col);
        //     count_foods++;
        //     foods = foods->next;
        // }
        // printf("%d\n", count_foods);
        printf("Actual antes en find_direction (%d, %d)\n", snake->row, snake->col);
        while (!next_move(board, dir))
        {
            dir = rand() % 4;
        }
        
        // dir = find_direction(board, next_move(board, dir), dir);
        // printf("%d\n", new_direction);
        enum Status status = move_snake(board, dir);
        waitFor(1);
        if (status == Failure) break;
        }
    return 0;
}