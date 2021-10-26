#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "snake.h"

// Removes from the list or return false
bool remove_from_list(PointList* elt, PointList** list) {
    PointList *previous, *current;

    if (*list == NULL) {
        return -1;
    }

    // The element is in the first position
    if (is_same_place(*list, elt)) {
        PointList * retval = NULL;
        PointList * next_node = NULL;

        if (*list == NULL) {
            return -1;
        }

        next_node = (*list)->next;
        retval = (*list);
        free(*list);
        *list = next_node;
        return true;
    }

    // The element is in other position
    previous = *list;
    current = (*list)->next;
    while (current) {
        if (is_same_place(current, elt)) {
            previous->next = current->next;
            free(current);
            return true;
        }

        previous = current;
        current  = current->next;
    }
    return false;
}

// The snake will move depending on the direction
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
        // Attach the beginning to the rest of the snake and grow up in 3 units;
        PointList* body1 = create_cell(board->snake->row, board->snake->col);
        body1->next = board->snake;
        board->snake = body1;
        PointList* body2 = create_cell(board->snake->row, board->snake->col);
        body2->next = board->snake;
        board->snake = body2; 
        beginning->next = board->snake;
        board->snake = beginning;
        board->score++;
        // Remove the food
        remove_from_list(beginning, &(board->foods));
        // Refill the board with foods
        if(no_food_in_board(board)) {
            int space_for_food = board->rows * board->cols - 
                snake_length_in_board(board->snake) - 1;
            for (int i = 0; i < 5 && i < space_for_food; i++)
            {
                add_new_food(board);
            }
        }    
        return Success;
    }

    // Attach the beginning to the rest of the snake
    beginning->next = board->snake;
    board->snake = beginning;

    // Cut off the end and lengthen the snake
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
        PointList* snake = board->snake;
        while (count_repeat > 0)
        {
            board->snake = board->snake->next;
            count_repeat--;
        }        
        board->snake->next = end->next->next;
        board->snake = snake;
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
// To check if the position is inside the board limits
bool is_out_of_board(Board* board, int row, int col) {
    return row < 0 || col < 0 || row >= board->rows || col >= board->cols;     
}
// Knowing an address returns the new position where the snake is going to move
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
// Returns a random box with upper bound the number of rows and columns that the board has
PointList* create_random_cell(int xmax, int ymax) {
    return create_cell(rand() % xmax, rand() % ymax);
}
// Add the 5 new fruits that the snake must eat to grow in random positions
void add_new_food(Board* board) {
    PointList* new_food;
    do {
        new_food = create_random_cell(board->rows, board->cols);
    }
    while (list_contains(new_food, board->foods) || list_contains(new_food, board->snake));
    new_food->next = board->foods;
    board->foods = new_food;
}
// Returns true if the position to which the snake wants to move exists a part of its body
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
// Create a cell
PointList* create_cell(int row, int col) {
    PointList* cell = malloc(sizeof(*cell));
    cell->row = row;
    cell->col = col;
    cell->next = NULL;
    return cell;
}
// Create the board
Board* create_board(PointList* snake, PointList* foods, int rows, int cols) {
    Board* board = malloc(sizeof(*board));
    board->rows = rows;
    board->cols = cols;
    board->snake = snake;
    board->foods = foods;
    board->score = 0;
    return board;
}
// Create the Snake
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
// To know if there is food left on the board to know whether to add new food or not
bool no_food_in_board(Board* board) {
    PointList* foods = board->foods;
    if(!foods)
        return true;
    return false;
}

int snake_length_in_board(PointList* snake) {
    int length = 0;
    PointList* psnake = snake;
    PointList *current, *prev;
    while (psnake->next)
    {
        current = create_cell(psnake->next->row, psnake->next->col);
        prev = create_cell(psnake->row, psnake->col);
        if(!is_same_place(prev, current))
            length++;
        psnake = psnake->next;
    }
    return length;
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
        PointList* beginning = next_move(board, *(ways + i)); 
        if(beginning) {
            return ways[i];
        }
    }
    return dir;
}

enum Direction find_direction(Board* board, enum Direction dir) {
    int dr[4] = { -1, 1, 0, 0};
    int dc[4] = { 0, 0, -1, 1};
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