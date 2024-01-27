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

#define ANOTHER_STONE(S) ((BLACK) + (WHITE) - (S))

typedef struct board {
    BOARD_STATE array[BOARD_WIDTH * BOARD_HEIGHT];
} BOARD;

typedef struct node {
    BOARD board;
    BOARD_STATE stone;
    char next_choices[FIELD_SIZE * FIELD_SIZE];	/* 数字を格納するが、メモリ節約のためchar型 */
    struct node **children;
} NODE;

/* boardで隣へ移動する際に添え字に足す数字（8方位） */
static int directions[8] = { 10, 11, 1, -9, -10, -11, -1, 9 };

static int count_next_choices(char *next_choices)
{
    int i = 0;
    while (next_choices[i] != -1) {
        i++;
    }
    return i;
}

static void push_next_choices(const int board_i, char *next_choices)
{
    int j = count_next_choices(next_choices);
    next_choices[j] = board_i;
    next_choices[j + 1] = -1;
    return;
}

static int count_reversibles_in_the_direction(const char board_i,
                                              const BOARD_STATE stone,
                                              const BOARD * board,
                                              const int dir_i)
{
    BOARD_STATE current;
    int count = 0;
    int i = board_i;
    while (1) {
        i += directions[dir_i];
        current = board->array[i];
        if (current == EMPTY || current == WALL) {
            return 0;
        } else if (current == stone) {
            return count;
        } else {
            count++;
        }
    }
}

static bool can_put_stone(const char board_i,
                          const BOARD_STATE stone, const BOARD * board)
{
    int d;
    if (board->array[board_i] != EMPTY) {
        return false;
    }

    for (d = 0; d < 8; d++) {
        if (count_reversibles_in_the_direction(board_i, stone, board, d)) {
            return true;
        }
    }
    return false;
}

static void search_next_choices(NODE * node)
{
    int x, y, d;
    for (x = 1; x < BOARD_HEIGHT - 1; x++) {
        for (y = 1; y < BOARD_WIDTH - 1; y++) {
            if (can_put_stone(BOARD_I(x, y), node->stone, &node->board)) {
                push_next_choices(BOARD_I(x, y), node->next_choices);
            }
        }
    }
}

static void reverse_stone_in_the_direction(const char board_i,
                                           BOARD_STATE stone,
                                           BOARD * board,
                                           const int direction_i)
{
    int i = board_i;
    i += directions[direction_i];
    while (board->array[i] != stone) {
        board->array[i] = stone;
        i += directions[direction_i];
    }
}

static void reverse_stone(const char board_i, BOARD_STATE stone,
                          BOARD * board)
{
    int direction_i;
    for (direction_i = 0; direction_i < 8; direction_i++) {
        if (count_reversibles_in_the_direction
            (board_i, stone, board, direction_i)
            != 0) {
            reverse_stone_in_the_direction(board_i, stone, board,
                                           direction_i);
        }
    }
}

static void put_stone(const char board_i, BOARD_STATE stone, BOARD * board)
{
    board->array[board_i] = stone;
    reverse_stone(board_i, stone, board);
}

static void print_next_choices(const char *next_choices)
{
    int j = 0;
    int i = next_choices[j];

    while (i != -1) {
        printf("(%d,%d) ", BOARD_X(i) - 1, BOARD_Y(i) - 1);
        j++;
        i = next_choices[j];
    }
    printf("\n");
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

static void print_board(const BOARD board)
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

static void create_child_node(NODE * node)
{
    char number_of_next_choices = count_next_choices(node->next_choices);
    char i;

    node->children = malloc(sizeof(NODE *) * number_of_next_choices);

    for (i = 0; i < number_of_next_choices; i++) {
        node->children[i] = malloc(sizeof(NODE));	/* 領域確保 */
        node->children[i]->stone = ANOTHER_STONE(node->stone);	/* 相手の石として登録 */
        node->children[i]->board = node->board;	/* 親の盤をコピー */
        put_stone(node->next_choices[i], node->stone,
                  &node->children[i]->board);
        node->children[i]->next_choices[0] = -1;	/* 次の手の配列を初期化 */
    }
}

static void init_root_node(NODE * root, const char *argv_1)
{
    int x, y;
    int i = 0;
    for (x = 0; x < BOARD_HEIGHT; x++) {
        for (y = 0; y < BOARD_WIDTH; y++) {
            if (x == 0 || x == BOARD_HEIGHT - 1 || y == 0
                || y == BOARD_WIDTH - 1) {
                root->board.array[BOARD_I(x, y)] = WALL;
            } else {
                root->board.array[BOARD_I(x, y)] = argv_1[i] - '0';
                i++;
            }
        }
    }

    root->stone = argv_1[65] - '0';
    root->next_choices[0] = -1;
    root->children = NULL;
}

int main(int argc, char *argv[])
{
    NODE root;
    char result_x, result_y;

    init_root_node(&root, argv[1]);
    search_next_choices(&root);

    print_board(root.board);
    print_next_choices(root.next_choices);

    create_child_node(&root);

    print_board(root.children[0]->board);
    print_board(root.children[1]->board);
    print_board(root.children[2]->board);
    print_board(root.children[3]->board);

    result_x = BOARD_X(root.next_choices[0]) - 1;
    result_y = BOARD_Y(root.next_choices[0]) - 1;

    printf("{x:%d,y:%d}\n", result_x, result_y);

    return 0;
}
