#include <stdbool.h>

enum Direction { Up, Down, Left, Right };
enum Status { Succes, Failure };

struct PointList 
{
    int x;
    int y;
    struct PointList* next;
};

typedef struct PointList PointList;

typedef struct {
    PointList* snake;
    PointList* froods;
    int xsize;
    int ysize;
} Board;

bool is_same_place(PointList* cells1, PointList* cells2);
void hello_world();