#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "../modules/board.h"

#define MAX_DEPTH (3)
#define ANOTHER_STONE(S) ((BLACK) + (WHITE) - (S))

struct timeval tv;

typedef struct node {
    Board board;
    int point;
    char depth;
    struct node **children;
} Node;

BoardState my_stone;
static BoardI result_i;

char location_score[BOARD_WIDTH * BOARD_HEIGHT] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 30, -12, 0, -1, -1, 0, -12, 30, 0,
    0, -12, -15, -3, -3, -3, -3, -15, -12, 0,
    0, 0, -3, 0, -1, -1, 0, -3, 0, 0,
    0, -1, -3, -1, -1, -1, -1, -3, -1, 0,
    0, -1, -3, -1, -1, -1, -1, -3, -1, 0,
    0, 0, -3, 0, -1, -1, 0, -3, 0, 0,
    0, -12, -15, -3, -3, -3, -3, -15, -12, 0,
    0, 30, -12, 0, -1, -1, 0, -12, 30, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static int evaluate(Node * node)
{
    return 0;
}

static void create_board(char *string, Board * board)
{
    char x, y, i;
    for (x = 0; x < BOARD_HEIGHT; x++) {
        for (y = 0; y < BOARD_WIDTH; y++) {
            if (x == 0 || x == BOARD_HEIGHT - 1 || y == 0
                || y == BOARD_WIDTH - 1) {
                board->array[GET_BOARD_I(x, y)] = WALL;
            } else {
                board->array[GET_BOARD_I(x, y)] = string[i] - '0';
                i++;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    Board board;
    create_board(argv[1], &board);
    print_board(&board);
    return 0;
}
