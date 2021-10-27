#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "snake.h"

// Removes from the list or return false
bool remove_from_list(PointList* elt, PointList** list) {
    PointList *currP, *prevP;
    prevP = NULL;

    // printf("(%d, %d)\n", (*list)->row, (*list)->col);
    // printf("(%d, %d)\n", currP->row, currP->col);
    
    for (currP = *list; currP != NULL; prevP = currP, currP = currP->next)
    {
        if(is_same_place(elt, currP)) {
            // It's the first element
            if(prevP == NULL)
                *list = currP->next;
            else
                prevP = currP->next;
            currP = NULL;
            // printf("(%d, %d)\n", currP->row, currP->col);
            // free(currP);
            // printf("(%d, %d)\n", currP->row, currP->col);
            return true;
        }
    }
    return false;
}

enum Status move_snake(Board* board, enum Direction dir) {
    // Create a new beginning. Check boundaries.
    PointList* beginning = next_move(board, dir);
    if(beginning == NULL)
        return Failure;
    
    // If we've gone backwards, don't do anything
    if (board->snake->next && is_same_place(beginning, board->snake->next)) {
        beginning->next = NULL;
        free(beginning);
            return Success;
    }

    // Check for collisions
    if (list_contains(beginning, board->snake))
        return Failure;

    // Check for food
    if (list_contains(beginning, board->foods)) {
        // Attach the beginning to the rest of the snake;
        PointList* body1 = create_cell(board->snake->row, board->snake->col);
        body1->next = board->snake;
        board->snake = body1;
        PointList* body2 = create_cell(board->snake->row, board->snake->col);
        body2->next = board->snake;
        board->snake = body2; 
        beginning->next = board->snake;
        board->snake = beginning;
        remove_from_list(beginning, &(board->foods));
        if(no_food_in_board(board)) {
            for (int i = 0; i < 5; i++)
            {
                add_new_food(board);
            }
            
        }
        // add_new_food(board);
    
        return Success;
    }

    // Attach the beginning to the rest of the snake
    beginning->next = board->snake;
    board->snake = beginning;

    // Cut off the end
    int count_repeat = 0;
    bool repeat = false;
    PointList* end = board->snake;
    while(end->next->next) {
        if(is_same_place(end, end->next)){
            repeat = true;
            break;
        }
        count_repeat++;
        end = end->next;
    }
    if(repeat){
        // PointList* to_free = end;
        PointList* snake = board->snake;
        while (count_repeat > 0)
        {
            board->snake = board->snake->next;
            count_repeat--;
        }
        
        board->snake->next = end->next->next;
        board->snake = snake;
        // free(to_free);
        // to_free = NULL;
    }
    else {
    free(end->next);
    end->next = NULL;
    }
    return Success;
}

bool is_same_place(PointList* cell1, PointList* cell2) {
    return cell1->row == cell2->row && cell1->col == cell2->col;
}

bool is_out_of_board(Board* board, int row, int col) {
    return row < 0 || col < 0 || row >= board->rows || col >= board->cols;     
}

PointList* next_move(Board* board, enum Direction dir) {
    PointList* snake = board -> snake;
    int new_row = snake->row;
    int new_col = snake->col;
    switch (dir)
    {
    case Up:
        new_row = snake->row - 1;
        break;
    case Down:
        new_row = snake->row + 1;
        break;
    case Left:
        new_col = snake->col - 1;
        break;
    case Right:
        new_col = snake->col + 1;
        break;
    }
    int is_snake_or_food = snake_or_food(board, new_row, new_col);
    if(is_out_of_board(board, new_row, new_col) || 
        ( is_snake_or_food != 0 && is_snake_or_food != 3)){
        return NULL;
    }
    else
        return create_cell(new_row, new_col);
}

PointList* create_random_cell(int xmax, int ymax) {
    return create_cell(rand() % xmax, rand() % ymax);
}

void add_new_food(Board* board) {
    PointList* new_food;
    do {
        new_food = create_random_cell(board->rows, board->cols);
    }
    while (list_contains(new_food, board->foods) || list_contains(new_food, board->snake));
    new_food->next = board->foods;
    board->foods = new_food;
}

bool list_contains(PointList* cell, PointList* list) {
    PointList* s = list;
    while (s)
    {
        if(is_same_place(cell, s))
            return true;
        s = s->next;
    }
    return false;
}

PointList* create_cell(int row, int col) {
    PointList* cell = malloc(sizeof(*cell));
    cell->row = row;
    cell->col = col;
    cell->next = NULL;
    return cell;
}

Board* create_board(PointList* snake, PointList* foods, int rows, int cols) {
    Board* board = malloc(sizeof(*board));
    board->rows = rows;
    board->cols = cols;
    board->snake = snake;
    board->foods = foods;
    return board;
}

PointList* create_snake(int rows, int cols) {
  PointList* a = create_cell(rows / 2, cols / 2);
  PointList* b = create_cell(a->row, a->col - 1);
  PointList* c = create_cell(a->row, a->col - 2);
  a->next = b;
  b->next = c;
  return a;
}

int snake_or_food(Board* board, int row, int col) {
    if(is_same_place(board->snake, create_cell(row, col)))
        return 1;
    PointList* rest_snake = board->snake->next;
    while (rest_snake)
    {
        if(is_same_place(rest_snake, create_cell(row, col)))
            return 2;
        rest_snake = rest_snake->next;
    }
    PointList* foods = board->foods;
    while (foods)
    {
        if(is_same_place(foods, create_cell(row, col)))
            return 3;
        foods = foods->next;
    }
    return 0;
}

bool no_food_in_board(Board* board) {
    PointList* foods = board->foods;
    if(!foods)
        return true;
    return false;
}