#ifndef _INCLUDE_BOARD_
#define _INCLUDE_BOARD_

#include <stdbool.h>

#define FIELD_SIZE (8)

#define BOARD_WIDTH ((FIELD_SIZE) + 2)
#define BOARD_HEIGHT ((FIELD_SIZE) + 2)

#define GET_BOARD_X(I) ((I) / (BOARD_HEIGHT))
#define GET_BOARD_Y(I) ((I) % (BOARD_HEIGHT))
#define GET_BOARD_I(X,Y) ((X) * (BOARD_HEIGHT) + (Y))

typedef enum board_state {
    EMPTY,
    WHITE,
    BLACK,
    WALL,
} BoardState;

typedef struct board {
    BoardState array[BOARD_WIDTH * BOARD_HEIGHT];
} Board;

typedef char BoardI;

extern BoardI directions[8];

extern char count_stone(const BoardState stone, const Board * board);
extern void init_board(Board * board);
extern void print_board_state_icon(const BoardState state);
extern void print_board(const Board * board);
extern char count_reverse_in_the_dir(const BoardI board_i,
                                     const BoardState stone,
                                     const char dir, const Board * board);
extern bool can_reverse(const BoardI board_i, const BoardState stone,
                        const Board * board);
extern void reverse_stone(const BoardI board_i, const BoardState stone,
                          Board * board);
extern bool put_stone(const BoardI board_i, const BoardState stone,
                      Board * board);
extern bool can_put_stone(const BoardI board_i, const BoardState stone,
                          Board * board);

#endif
