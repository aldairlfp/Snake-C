#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "snake.h"

// Removes from the list or return false
bool remove_from_list(PointList* elt, PointList** list) {
    PointList *currP, *prevP;
    prevP = NULL;
    
    for (currP = *list; currP != NULL; prevP = currP, currP = currP->next)
    {
        if(is_same_place(elt, currP)) {
            // It's the first element
            if(prevP == NULL)
                *list = currP->next;
            else 
                prevP = currP->next;
            free(currP);
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
        beginning->next = board->snake;
        PointList* body1 = beginning;
        body1->next = board->snake;
        PointList* body2 = beginning;
        body2->next = board->snake;
        board->snake = body2;
        remove_from_list(beginning, &(board->foods));
        // add_new_food(board);
    
        return Success;
    }

    // Attach the beginning to the rest of the snake
    beginning->next = board->snake;
    board->snake = beginning;

    // Cut off the end
    PointList* end = board->snake;
    while(end->next->next) {
        end = end->next;
    }
    free(end->next);
    end->next = NULL;

    return Success;
}

bool is_same_place(PointList* cell1, PointList* cell2) {
    return cell1->x == cell2->x && cell1->y == cell2->y;
}

bool is_on_board(Board* board, int x, int y) {
    return x < 0 || y < 0 || x >= board->xsize || y >= board->ysize;     
}

PointList* next_move(Board* board, enum Direction dir) {
    PointList* snake = board -> snake;
    int new_x = snake->x;
    int new_y = snake->y;
    switch (dir)
    {
    case Up:
        new_y = snake->y + 1;
        break;
    case Down:
        new_y = snake->y - 1;
        break;
    case Left:
        new_x = snake->x - 1;
        break;
    case Right:
        new_x = snake->x + 1;
        break;
    }
    if(is_on_board(board, new_x, new_y))
        return NULL;
    else
        return create_cell(new_x, new_y);
}

PointList* create_random_cell(int xmax, int ymax) {
    return create_cell(rand() % xmax, rand() % ymax);
}

void add_new_food(Board* board) {
    PointList* new_food;
    do {
        // PointList* point = create_random_cell(board->xsize, board->ysize);
        // printf("%d, ", point->x);
        // printf("%d\n", point->y);
        new_food = create_random_cell(board->xsize, board->ysize);
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

PointList* create_cell(int x, int y) {
    PointList* cell = malloc(sizeof(*cell));
    cell->x = x;
    cell->y = y;
    cell->next = NULL;
    return cell;
}

Board* create_board(PointList* snake, PointList* foods, int xsize, int ysize) {
    Board* board = malloc(sizeof(*board));
    board->xsize = xsize;
    board->ysize = ysize;
    board->snake = snake;
    board->foods = foods;
    return board;
}

PointList* create_snake(int xmax, int ymax) {
  PointList* a = create_cell(xmax / 2, ymax / 2);
  PointList* b = create_cell(a->x - 1, a->y);
  PointList* c = create_cell(a->x - 2, a->y);
  a->next = b;
  b->next = c;
  return a;
}

int snake_or_food(Board* board, int x, int y) {
    if(is_same_place(board->snake, create_cell(x, y)))
        return 1;
    PointList* rest_snake = board->snake->next;
    while (rest_snake)
    {
        if(is_same_place(rest_snake, create_cell(x, y)))
            return 2;
        rest_snake = rest_snake->next;
    }
    PointList* foods = board->foods;
    while (foods)
    {
        if(is_same_place(foods, create_cell(x, y)))
            return 3;
        foods = foods->next;
    }
    return 0;
}

void grow_snake(PointList* snake, int size){

}