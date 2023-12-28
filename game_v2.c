#include <stdio.h>
#include <stdbool.h>

#define DEBUG_PRINT (true)
#define GAME_ANNOUNCEMENT_PRINT (true)
#define MAX_LENGTH_OF_PLAYER_NAME (30)
#define SIZE (8)
#define WIDTH ((SIZE) + 2)
#define HEIGHT ((SIZE) + 2)

typedef enum board_state {
    BLACK,                      /* 0: 黒石 */
    WHITE,                      /* 1: 白石 */
    EMPTY,                      /* 2: 空白 */
    WALL                        /* 3: 壁 */
} BOARD_STATE;

typedef enum player_type {
    HUMAN,
    C_AGENT,
    PYTHON_AGENT,
} PLAYER_TYPE;

typedef struct player {
    BOARD_STATE stone;
    PLAYER_TYPE player_type;
    char name[MAX_LENGTH_OF_PLAYER_NAME];
} PLAYER;

typedef struct game_result {
    PLAYER winner;
    int turn_count;
} GAME_RESTLT;

/* 盤 */
/* 上下一行ずつ・左右一列ずつを壁とする */
static BOARD_STATE board[WIDTH * HEIGHT];

/* boardで隣へ移動する際に添え字に足す数字（8方位） */
/* 下, 右下, 右, 右上, 上, 左上, 左, 左下 */
static const int directions[8] = { 10, 11, 1, -9, -10, -11, -1, 9 };

/* 次に打つことができる場所 */
/* -1は番兵 */
static int next_choices[SIZE * SIZE] = { -1 };

/* 2次元座標を受け取りboardの添え字を返す */
static int get_board_i(const int x, const int y)
{
    return x * HEIGHT + y;
}

/* boardの添え字を受け取りx座標を返す */
static int get_board_x(const int i)
{
    return i / HEIGHT;
}

/* boardの添え字を受け取りy座標を返す */
static int get_board_y(const int i)
{
    return i % HEIGHT;
}

/* 盤を初期化する */
static void init_board(void)
{
    int x, y;
    /* 壁と空白で埋める */
    for (x = 0; x < HEIGHT; x++) {
        for (y = 0; y < WIDTH; y++) {
            if (x == 0 || x == HEIGHT - 1 || y == 0 || y == WIDTH - 1) {
                board[get_board_i(x, y)] = WALL;
            } else {
                board[get_board_i(x, y)] = EMPTY;
            }
        }
    }
    /* 初期配置 */
    board[get_board_i(HEIGHT / 2, WIDTH / 2)] = WHITE;
    board[get_board_i(HEIGHT / 2 - 1, WIDTH / 2 - 1)] = WHITE;
    board[get_board_i(HEIGHT / 2 - 1, WIDTH / 2)] = BLACK;
    board[get_board_i(HEIGHT / 2, WIDTH / 2 - 1)] = BLACK;
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

static void get_location(int *x, int *y)
{
    /* 仮の値 */
    *x = 3;
    *y = 5;
}

static bool is_finished()
{
    return false;
}

static void proceed_game(PLAYER * player_a,
                         PLAYER * player_b, PLAYER * winner)
{
    int turn_count = 1;
    PLAYER *current_turn_player;

    init_board();
    while (is_finished() == false) {
        printf("%d\n", turn_count);
        current_turn_player = (turn_count % 2 != 0) ? player_a : player_b;
        if (GAME_ANNOUNCEMENT_PRINT != false) {
            printf("%sの番です\n", current_turn_player->name);
        }
        turn_count++;
    }
    print_board();
}

static void set_player(PLAYER * player)
{
    printf("■ プレイヤータイプを選択\n");
    printf("   %d: 人間\n", HUMAN);
    printf("   %d: AI（C言語）\n", C_AGENT);
    printf("   %d: AI（Python）\n", PYTHON_AGENT);
    printf(">> ");
    scanf("%d", &player->player_type);
    printf("「%d」が選択されました\n", player->player_type);
    printf("\n");

    printf("■ プレイヤー名の設定\n");
    printf(">> ");
    scanf("%s", player->name);
    printf("「%s」が入力されました\n", player->name);
    printf("\n");
}

int main(void)
{
    PLAYER player_a;            /* 先攻 */
    PLAYER player_b;            /* 後攻 */
    PLAYER *winner;

    player_a.stone = BLACK;
    player_b.stone = WHITE;

    printf("== 先攻 ==\n");
    set_player(&player_a);
    printf("== 後攻 ==\n");
    set_player(&player_b);

    proceed_game(&player_a, &player_b, winner);

    return 0;
}
