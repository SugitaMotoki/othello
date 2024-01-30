#ifndef _INCLUDE_BOARD_
#define _INCLUDE_BOARD_

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

extern void count_stone(char *black, char *white, const Board * board);
extern void init_board(Board * board);
static void print_board_state_icon(const BoardState state);
extern void print_board(const Board * board);

#endif
