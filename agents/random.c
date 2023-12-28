#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#define DEBUG_PRINT true
#define SIZE (8)                /* オセロなので8*8 */
#define WIDTH (SIZE + 2)
#define HEIGHT (SIZE + 2)

/* ランダム値生成で使用する型 */
struct timeval tv;

typedef enum board_state {
    black,                      /* 0: 黒石 */
    white,                      /* 1: 白石 */
    empty,                      /* 2: 空白 */
    wall                        /* 3: 壁 */
} BOARD_STATE;

/* 盤 */
/* 上下一行ずつ・左右一列ずつを壁とする */
BOARD_STATE board[WIDTH * HEIGHT];

/* boardで隣へ移動する際に添え字に足す数字（8方位） */
/* 下, 右下, 右, 右上, 上, 左上, 左, 左下 */
int directions[8] = { 10, 11, 1, -9, -10, -11, -1, 9 };

/* 次に打つことができる場所*/
int next_choices[SIZE * SIZE] = { -1 };

/* 2次元座標を受け取りboardの添え字を返す */
int get_board_i(const int x, const int y)
{
    return x * HEIGHT + y;
}

/* boardの添え字を受け取りx座標を返す関数 */
int get_board_x(const int i)
{
    return i / HEIGHT;
}

/* boardの添え字を受け取りy座標を返す関数 */
int get_board_y(const int i)
{
    return i % HEIGHT;
}

int count_next_choices(void)
{
    int i = 0;
    while (next_choices[i] != -1) {
        i++;
    }
    return i;
}

void push_next_choices(const int board_i)
{
    int j = 0;
    while (next_choices[j] != -1) {
        j++;
    }
    next_choices[j] = board_i;
    next_choices[j + 1] = -1;
    return;
}

int count_reversibles_one_dir(const int x, const int y, BOARD_STATE stone,
                              const int dir_i)
{
    BOARD_STATE current;
    int count = 0;
    int i = get_board_i(x, y);
    while (1) {
        i += directions[dir_i];
        current = board[i];
        if (current == empty || current == wall) {
            return 0;
        } else if (current == stone) {
            return count;
        } else {
            count++;
        }
    }
}

bool can_put_stone(const int x, const int y, BOARD_STATE stone)
{
    int d;
    if (board[get_board_i(x, y)] != empty) {
        return false;
    }

    for (d = 0; d < 8; d++) {
        if (count_reversibles_one_dir(x, y, stone, d)) {
            return true;
        }
    }
    return false;
}

void search_next_choices(BOARD_STATE stone)
{
    int x, y, d;
    for (x = 1; x < HEIGHT - 1; x++) {
        for (y = 1; y < WIDTH - 1; y++) {
            if (can_put_stone(x, y, stone)) {
                push_next_choices(get_board_i(x, y));
            }
        }
    }
}

/* 盤を空白と壁で初期化する */
void init_board(void)
{
    int x, y;
    for (x = 0; x < HEIGHT; x++) {
        for (y = 0; y < WIDTH; y++) {
            if (x == 0 || x == HEIGHT - 1 || y == 0 || y == WIDTH - 1) {
                board[get_board_i(x, y)] = wall;
            } else {
                board[get_board_i(x, y)] = empty;
            }
        }
    }
}

/* 最初に入力された数字列をboardに落とし込む */
void set_board(const char *board_state_str)
{
    int x, y;
    int i = 0;
    for (x = 1; x < HEIGHT - 1; x++) {
        for (y = 1; y < WIDTH - 1; y++) {
            board[get_board_i(x, y)] = board_state_str[i] - '0';
            i++;
        }
    }
}

int print_next_choices()
{
    int j = 0;
    int i = next_choices[j];

    while (i != -1) {
        printf("%d ", i);
        j++;
        i = next_choices[j];
    }
    printf("\n");
    return j;
}

void print_board_state_icon(const BOARD_STATE state)
{
    switch (state) {
    case black:
        printf("●");
        break;
    case white:
        printf("○");
        break;
    case empty:
        printf(" ");
        break;
    case wall:
        printf("w");
        break;
    default:
        printf("!");
        break;
    }
}

void print_board(void)
{
    int x, y;

    printf("  y\nx ");
    for (y = 1; y < WIDTH - 1; y++) {
        printf(" %d", y);
    }
    printf("\n");

    for (x = 1; x < HEIGHT - 1; x++) {
        printf(" %d|", x);
        for (y = 1; y < WIDTH - 1; y++) {
            print_board_state_icon(board[get_board_i(x, y)]);
            printf("|");
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    BOARD_STATE stone;
    int r = 0;

    if (argc != 3) {
        printf("引数不正\n");
        exit(1);
    }

    stone = argv[1][0] - '0';

    init_board();
    set_board(argv[2]);

    if (DEBUG_PRINT) {
        print_board();
        print_board_state_icon(stone);
        printf(" ← Next\n");
        print_board_state_icon(1 - stone);
        printf("\n");
    }

    search_next_choices(stone);

    /* ランダム値生成 */
    gettimeofday(&tv, NULL);
    srand(tv.tv_sec + tv.tv_usec);
    r = (int) (rand() * ((count_next_choices() - 1) + 1.0) /
               (RAND_MAX + 1.0));

    if (DEBUG_PRINT) {
        printf("Choices: ");
        print_next_choices();
    }

    printf("%d %d\n", get_board_x(next_choices[r]),
           get_board_y(next_choices[r]));

    return 0;
}
