#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEBUG_PRINT (true)
#define FIELD_SIZE (8)          /* オセロのルール上の盤の大きさ */
#define BOARD_WIDTH ((FIELD_SIZE) + 2)	/* 両端の壁を含めた幅 */
#define BOARD_HEIGHT ((FIELD_SIZE) + 2)	/* 両端の壁を含めた高さ */
#define MAX_DEPTH (5)
#define GET_BOARD_I(X,Y) ((X) * (BOARD_HEIGHT) + (Y))
#define GET_BOARD_X(I) ((I) / (BOARD_HEIGHT))
#define GET_BOARD_Y(I) ((I) % (BOARD_HEIGHT))

typedef enum board_state {
    EMPTY,                      /* 0: 空白 */
    WHITE,                      /* 1: 白石 */
    BLACK,                      /* 2: 黒石 */
    WALL                        /* 3: 壁 */
} BoardState;

#define ANOTHER_STONE(S) ((BLACK) + (WHITE) - (S))

typedef struct board {
    BoardState array[BOARD_WIDTH * BOARD_HEIGHT];
} Board;

typedef char BoardI;

typedef struct node {
    BoardState stone;
    Board board;
    BoardI next_choices[FIELD_SIZE * FIELD_SIZE];	/* 数字を格納するが、メモリ節約のためchar型 */
    char depth;
    char own_stone;
    char enemys_stone;
    struct node **children;
} Node;

/* boardで隣へ移動する際に添え字に足す数字（8方位） */
static BoardI directions[8] = { 10, 11, 1, -9, -10, -11, -1, 9 };

static BoardI result_i;

static char count_stone(const BoardState stone, const Board board)
{
    char x, y;
    char count = 0;
    for (x = 1; x < BOARD_HEIGHT - 1; x++) {
        for (y = 1; y < BOARD_WIDTH - 1; y++) {
            if (board.array[GET_BOARD_I(x, y)] == stone) {
                count++;
            }
        }
    }
    return count;
}

static char count_next_choices(BoardI * next_choices)
{
    char i = 0;
    while (next_choices[i] != -1) {
        i++;
    }
    return i;
}

static void push_next_choices(const BoardI board_i, BoardI * next_choices)
{
    char j = count_next_choices(next_choices);
    next_choices[j] = board_i;
    next_choices[j + 1] = -1;
    return;
}

static char count_reversibles_in_the_direction(const BoardI i,
                                               const BoardState stone,
                                               const Board * board,
                                               const char dir_i)
{
    BoardState current;
    BoardI j = i;
    char count = 0;

    while (1) {
        j += directions[dir_i];
        current = board->array[j];
        if (current == EMPTY || current == WALL) {
            return 0;
        } else if (current == stone) {
            return count;
        } else {
            count++;
        }
    }
}

static bool can_put_stone(const BoardI board_i,
                          const BoardState stone, const Board * board)
{
    char dir_i;
    if (board->array[board_i] != EMPTY) {
        return false;
    }

    for (dir_i = 0; dir_i < 8; dir_i++) {
        if (count_reversibles_in_the_direction
            (board_i, stone, board, dir_i)) {
            return true;
        }
    }
    return false;
}

static void search_next_choices(Node * node)
{
    char x, y;
    for (x = 1; x < BOARD_HEIGHT - 1; x++) {
        for (y = 1; y < BOARD_WIDTH - 1; y++) {
            if (can_put_stone
                (GET_BOARD_I(x, y), node->stone, &node->board)) {
                push_next_choices(GET_BOARD_I(x, y), node->next_choices);
            }
        }
    }
}

static void reverse_stone_in_the_direction(const BoardI i,
                                           BoardState stone,
                                           Board * board, const char dir_i)
{
    BoardI j = i;
    j += directions[dir_i];
    while (board->array[j] != stone) {
        board->array[j] = stone;
        j += directions[dir_i];
    }
}

static void reverse_stone(const BoardI board_i, BoardState stone,
                          Board * board)
{
    char dir_i;
    for (dir_i = 0; dir_i < 8; dir_i++) {
        if (count_reversibles_in_the_direction
            (board_i, stone, board, dir_i)
            != 0) {
            reverse_stone_in_the_direction(board_i, stone, board, dir_i);
        }
    }
}

static void put_stone(const BoardI i, BoardState stone, Board * board)
{
    board->array[i] = stone;
    reverse_stone(i, stone, board);
}

static void print_next_choices(const BoardI * next_choices)
{
    char j = 0;
    BoardI i = next_choices[j];

    while (i != -1) {
        printf("(%d,%d) ", GET_BOARD_X(i) - 1, GET_BOARD_Y(i) - 1);
        j++;
        i = next_choices[j];
    }
    printf("\n");
}

static void print_board_state_icon(const BoardState state)
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

static void print_board(const Board board)
{
    char x, y;

    printf("  y\nx ");
    for (y = 1; y < BOARD_WIDTH - 1; y++) {
        printf(" %d", y - 1);
    }
    printf("\n");

    for (x = 1; x < BOARD_HEIGHT - 1; x++) {
        printf(" %d|", x - 1);
        for (y = 1; y < BOARD_WIDTH - 1; y++) {
            print_board_state_icon(board.array[GET_BOARD_I(x, y)]);
            printf("|");
        }
        printf("\n");
    }
}

/* 子ノードを作成し、作成数を返す */
static char create_child_node(Node * node)
{
    char number_of_next_choices = count_next_choices(node->next_choices);
    char i;
    Node *new_node;

    node->children = malloc(sizeof(Node *) * number_of_next_choices);

    for (i = 0; i < number_of_next_choices; i++) {
        new_node = malloc(sizeof(Node));
        new_node->stone = ANOTHER_STONE(node->stone);
        new_node->depth = node->depth + 1;
        new_node->board = node->board;
        put_stone(node->next_choices[i], node->stone, &new_node->board);
        new_node->next_choices[0] = -1;
        search_next_choices(new_node);
        node->children[i] = new_node;
    }
}

static void run_node(Node * node)
{
    char i;
    char number_of_child;

    if (node->depth == MAX_DEPTH) {
        return;
    }

    number_of_child = create_child_node(node);

    for (i = 0; i < number_of_child; i++) {
        run_node(node->children[i]);
    }
}

static void init_root_node(Node * root, const char *argv_1)
{
    char x, y;
    BoardI i = 0;
    for (x = 0; x < BOARD_HEIGHT; x++) {
        for (y = 0; y < BOARD_WIDTH; y++) {
            if (x == 0 || x == BOARD_HEIGHT - 1 || y == 0
                || y == BOARD_WIDTH - 1) {
                root->board.array[GET_BOARD_I(x, y)] = WALL;
            } else {
                root->board.array[GET_BOARD_I(x, y)] = argv_1[i] - '0';
                i++;
            }
        }
    }

    root->stone = argv_1[65] - '0';
    root->own_stone = count_stone(root->stone, root->board);
    root->enemys_stone =
        count_stone(ANOTHER_STONE(root->stone), root->board);
    root->depth = 0;
    root->next_choices[0] = -1;
    root->children = NULL;
}

int main(int argc, char *argv[])
{
    Node root;

    init_root_node(&root, argv[1]);
    search_next_choices(&root);
    result_i = root.next_choices[0];

    run_node(&root);

    printf("{x:%d,y:%d}\n", GET_BOARD_X(result_i) - 1,
           GET_BOARD_Y(result_i) - 1);

    return 0;
}
