#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#define DEBUG_PRINT true
#define SIZE (8)                /* オセロなので8*8 */
#define WIDTH ((SIZE) + 2)
#define HEIGHT ((SIZE) + 2)
#define BOARD_I(X,Y) ((X) * (HEIGHT) + (Y))
#define BOARD_X(I) ((I) / (HEIGHT))
#define BOARD_Y(I) ((I) % (HEIGHT))

/* ランダム値生成で使用する型 */
struct timeval tv;

typedef enum board_state {
    EMPTY,                      /* 0: 空白 */
    WHITE,                      /* 1: 白石 */
    BLACK,                      /* 2: 黒石 */
    WALL                        /* 3: 壁 */
} BOARD_STATE;

/* 盤 */
/* 上下一行ずつ・左右一列ずつを壁とする */
BOARD_STATE board[WIDTH * HEIGHT];

/* boardで隣へ移動する際に添え字に足す数字（8方位） */
/* 下, 右下, 右, 右上, 上, 左上, 左, 左下 */
int directions[8] = { 10, 11, 1, -9, -10, -11, -1, 9 };

/* 次に打つことができる場所*/
int next_choices[SIZE * SIZE] = { -1 };

static int count_next_choices(void)
{
    int i = 0;
    while (next_choices[i] != -1) {
        i++;
    }
    return i;
}

static void push_next_choices(const int board_i)
{
    int j = 0;
    while (next_choices[j] != -1) {
        j++;
    }
    next_choices[j] = board_i;
    next_choices[j + 1] = -1;
    return;
}

static int count_reversibles_one_dir(const int x, const int y,
                                     BOARD_STATE stone, const int dir_i)
{
    BOARD_STATE current;
    int count = 0;
    int i = BOARD_I(x, y);
    while (1) {
        i += directions[dir_i];
        current = board[i];
        if (current == EMPTY || current == WALL) {
            return 0;
        } else if (current == stone) {
            return count;
        } else {
            count++;
        }
    }
}

static bool can_put_stone(const int x, const int y, BOARD_STATE stone)
{
    int d;
    if (board[BOARD_I(x, y)] != EMPTY) {
        return false;
    }

    for (d = 0; d < 8; d++) {
        if (count_reversibles_one_dir(x, y, stone, d)) {
            return true;
        }
    }
    return false;
}

static void search_next_choices(BOARD_STATE stone)
{
    int x, y, d;
    for (x = 1; x < HEIGHT - 1; x++) {
        for (y = 1; y < WIDTH - 1; y++) {
            if (can_put_stone(x, y, stone)) {
                push_next_choices(BOARD_I(x, y));
            }
        }
    }
}

/* 盤を空白と壁で初期化する */
static void init_board(void)
{
    int x, y;
    for (x = 0; x < HEIGHT; x++) {
        for (y = 0; y < WIDTH; y++) {
            if (x == 0 || x == HEIGHT - 1 || y == 0 || y == WIDTH - 1) {
                board[BOARD_I(x, y)] = WALL;
            } else {
                board[BOARD_I(x, y)] = EMPTY;
            }
        }
    }
}

/* 最初に入力された数字列をboardに落とし込む */
static void set_board(const char *board_state_str)
{
    int x, y;
    int i = 0;
    for (x = 1; x < HEIGHT - 1; x++) {
        for (y = 1; y < WIDTH - 1; y++) {
            board[BOARD_I(x, y)] = board_state_str[i] - '0';
            i++;
        }
    }
}

static int print_next_choices()
{
    int j = 0;
    int i = next_choices[j];

    while (i != -1) {
        printf("(%d,%d) ", BOARD_X(i) - 1, BOARD_Y(i) - 1);
        j++;
        i = next_choices[j];
    }
    printf("\n");
    return j;
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

static void print_board(void)
{
    int x, y;

    printf("  y\nx ");
    for (y = 1; y < WIDTH - 1; y++) {
        printf(" %d", y - 1);
    }
    printf("\n");

    for (x = 1; x < HEIGHT - 1; x++) {
        printf(" %d|", x - 1);
        for (y = 1; y < WIDTH - 1; y++) {
            print_board_state_icon(board[BOARD_I(x, y)]);
            printf("|");
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    BOARD_STATE stone;
    int random_i = 0;
    int next_i;
    int next_x;
    int next_y;

    if (argc != 2) {
        printf("引数不正\n");
        exit(1);
    }

    stone = argv[1][65] - '0';

    init_board();
    set_board(argv[1]);

    if (DEBUG_PRINT) {
        print_board();
        print_board_state_icon(stone);
        printf(" ← Next\n");
        print_board_state_icon(BLACK + WHITE - stone);
        printf("\n");
    }

    search_next_choices(stone);

    /* ランダム値生成 */
    gettimeofday(&tv, NULL);
    srand(tv.tv_sec + tv.tv_usec);
    random_i = (int) (rand() * ((count_next_choices() - 1) + 1.0) /
                      (RAND_MAX + 1.0));

    next_i = next_choices[random_i];
    next_x = BOARD_X(next_i) - 1;
    next_y = BOARD_Y(next_i) - 1;

    if (DEBUG_PRINT) {
        printf("Choices: ");
        print_next_choices();
    }

    /* JSONでの結果出力 */
    printf("{x:%d,y:%d}\n", next_x, next_y);

    return 0;
}
