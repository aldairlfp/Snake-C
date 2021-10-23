#include <stdbool.h>

enum Direction { Up, Down, Left, Right };
enum Status { Success, Failure };

struct PointList 
{
    int row;
    int col;
    struct PointList* next;
};

typedef struct PointList PointList;

typedef struct {
    PointList* snake;
    PointList* foods;
    int rows;
    int cols;
} Board;

bool is_same_place(PointList* cells1, PointList* cells2);
enum Status move_snake(Board* board, enum Direction dir);
PointList* next_move(Board* board, enum Direction dir);
PointList* create_cell(int x, int y);
PointList* create_random_cell(int x, int y);
PointList* create_snake(int rows, int cols);
Board* create_board(PointList* sanke, PointList* food, int rows, int cols);
bool list_contains(PointList* cell, PointList* list);
bool remove_from_list(PointList* elt, PointList** list);
bool is_out_of_board(Board* board, int x, int y);
void add_new_food(Board* board);
int snake_or_food(Board* board, int x, int y);
void grow_snake(PointList* snake, int size);