#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEBUG_PRINT (true)
#define FIELD_SIZE (8)          /* オセロのルール上の盤の大きさ */
#define BOARD_WIDTH ((FIELD_SIZE) + 2)	/* 両端の壁を含めた幅 */
#define BOARD_HEIGHT ((FIELD_SIZE) + 2)	/* 両端の壁を含めた高さ */
#define BOARD_I(X,Y) ((X) * (BOARD_HEIGHT) + (Y))
#define BOARD_X(I) ((I) / (BOARD_HEIGHT))
#define BOARD_Y(I) ((I) % (BOARD_HEIGHT))

typedef enum board_state {
    EMPTY,                      /* 0: 空白 */
    WHITE,                      /* 1: 白石 */
    BLACK,                      /* 2: 黒石 */
    WALL                        /* 3: 壁 */
} BOARD_STATE;

typedef struct board {
    BOARD_STATE array[BOARD_WIDTH * BOARD_HEIGHT];
} BOARD;

typedef struct node {
    BOARD_STATE stone;
    BOARD board;
} NODE;

/* boardで隣へ移動する際に添え字に足す数字（8方位） */
static int directions[8] = { 10, 11, 1, -9, -10, -11, -1, 9 };

static void init_root_node(NODE * root, const char *first_board_str)
{
    int x, y;
    int i = 0;
    for (x = 0; x < BOARD_HEIGHT; x++) {
        for (y = 0; y < BOARD_WIDTH; y++) {
            if (x == 0 || x == BOARD_HEIGHT - 1 || y == 0
                || y == BOARD_WIDTH - 1) {
                root->board.array[BOARD_I(x, y)] = WALL;
            } else {
                root->board.array[BOARD_I(x, y)] =
                    first_board_str[i] - '0';
                i++;
            }
        }
    }
}

static void print_board_state_icon(const BOARD_STATE state)
{
    switch (state) {
    case BLACK:
        printf("●");
        break;
    case WHITE:
        printf("○");
        break;
    case EMPTY:
        printf(" ");
        break;
    case WALL:
        printf("w");
        break;
    default:
        printf("!");
        break;
    }
}

static void print_board(BOARD board)
{
    int x, y;

    printf("  y\nx ");
    for (y = 1; y < BOARD_WIDTH - 1; y++) {
        printf(" %d", y - 1);
    }
    printf("\n");

    for (x = 1; x < BOARD_HEIGHT - 1; x++) {
        printf(" %d|", x - 1);
        for (y = 1; y < BOARD_WIDTH - 1; y++) {
            print_board_state_icon(board.array[BOARD_I(x, y)]);
            printf("|");
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    NODE root;
    BOARD_STATE stone;

    stone = argv[1][65] - '0';

    init_root_node(&root, argv[1]);

    print_board(root.board);

    return 0;
}
