#include <stdio.h>
#include <stdbool.h>

#define DEBUG_PRINT (true)
#define GAME_ANNOUNCEMENT_PRINT (true)
#define MAX_LENGTH_OF_PLAYER_NAME (30)
#define SIZE (8)
#define WIDTH ((SIZE) + 2)
#define HEIGHT ((SIZE) + 2)
#define PASS_BOARD_I (91)

typedef enum board_state {
    BLACK,                      /* 0: 黒石 */
    WHITE,                      /* 1: 白石 */
    EMPTY,                      /* 2: 空白 */
    WALL                        /* 3: 壁 */
} BOARD_STATE;

typedef enum player_type {
    HUMAN,
    C_AGENT,
    PYTHON_AGENT
} PLAYER_TYPE;

typedef struct player {
    BOARD_STATE stone;
    PLAYER_TYPE player_type;
    char name[MAX_LENGTH_OF_PLAYER_NAME];
    int next_choices[SIZE * SIZE];
    bool is_passed;
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

static int print_next_choices(PLAYER * player)
{
    int j = 0;
    int i = player->next_choices[j];

    while (i != -1) {
        printf("(%d,%d) ", get_board_x(i), get_board_y(i));
        j++;
        i = player->next_choices[j];
    }
    printf("\n");
    return j;
}

static int count_reversibles_in_the_direction(const int x, const int y,
                                              BOARD_STATE stone,
                                              const int dir_i)
{
    BOARD_STATE current;
    int count = 0;
    int i = get_board_i(x, y);
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
    if (board[get_board_i(x, y)] != EMPTY) {
        return false;
    }

    for (d = 0; d < 8; d++) {
        if (count_reversibles_in_the_direction(x, y, stone, d)) {
            return true;
        }
    }
    return false;
}

static void push_next_choices(PLAYER * player, const int board_i)
{
    int j = 0;
    while (player->next_choices[j] != -1) {
        j++;
    }
    player->next_choices[j] = board_i;
    player->next_choices[j + 1] = -1;
    return;
}

static void search_next_choices(PLAYER * player)
{
    int x, y;
    player->next_choices[0] = -1;
    for (x = 1; x < HEIGHT - 1; x++) {
        for (y = 1; y < WIDTH - 1; y++) {
            if (can_put_stone(x, y, player->stone) != false) {
                push_next_choices(player, get_board_i(x, y));
            }
        }
    }
}

static void reverse_stone_in_the_direction(const int x, const int y,
                                           BOARD_STATE stone,
                                           const int direction_i)
{
    int i = get_board_i(x, y);
    i += directions[direction_i];
    while (board[i] != stone) {
        board[i] = stone;
        i += directions[direction_i];
    }
}

static void reverse_stone(const int x, const int y, BOARD_STATE stone)
{
    int direction_i;
    for (direction_i = 0; direction_i < 8; direction_i++) {
        if (count_reversibles_in_the_direction(x, y, stone, direction_i) !=
            0) {
            reverse_stone_in_the_direction(x, y, stone, direction_i);
        }
    }
}

static void put_stone(const int x, const int y, BOARD_STATE stone)
{
    int i = get_board_i(x, y);
    board[i] = stone;
    reverse_stone(x, y, stone);
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

/* PASSの場合はfalseを返す*/
static bool get_location(PLAYER * player, int *x, int *y)
{
    while (true) {
        printf(">> ");
        scanf("%d%d", x, y);
        if (get_board_i(*x, *y) == PASS_BOARD_I) {
            if (player->next_choices[0] == -1) {
                return false;
            } else {
                printf
                    ("<< 置ける場所があるためパスできません\n");
            }
        } else if (can_put_stone(*x, *y, player->stone) == false) {
            printf("<< ");
            print_board_state_icon(player->stone);
            printf("を(%d,%d)に置くことはできません\n", *x,
                   *y);
        } else {
            break;
        }
    }
    return true;
}

static bool is_finished(PLAYER * player_a, PLAYER * player_b)
{
    if (player_a->is_passed != false && player_b->is_passed != false) {
        return true;
    }
    return false;
}

static void proceed_game(PLAYER * player_a,
                         PLAYER * player_b, PLAYER * winner)
{
    int turn_count = 0;
    PLAYER *current_turn_player;
    int x, y;
    bool is_passed;

    init_board();
    printf("■ ゲーム開始\n");
    while (is_finished(player_a, player_b) == false) {
        turn_count++;
        current_turn_player = (turn_count % 2 != 0) ? player_a : player_b;
        search_next_choices(current_turn_player);
        if (GAME_ANNOUNCEMENT_PRINT != false) {
            printf("== ターン: %d ==\n", turn_count);
            print_board();
            printf("プレイヤー: %s\n", current_turn_player->name);
            printf("石: ");
            print_board_state_icon(current_turn_player->stone);
            printf("\n");
            printf("置ける場所: ");
            print_next_choices(current_turn_player);
            printf("パス: (%d,%d)\n", get_board_x(PASS_BOARD_I),
                   get_board_y(PASS_BOARD_I));
            printf
                ("<< 座標を入力してください（例：3 4）\n");
        }
        is_passed = !get_location(current_turn_player, &x, &y);
        if (is_passed == false) {
            current_turn_player->is_passed = false;
            put_stone(x, y, current_turn_player->stone);
        } else {
            current_turn_player->is_passed = true;
        }
    }
    if (GAME_ANNOUNCEMENT_PRINT != false) {
        printf("■ ゲーム終了\n");
    }
    winner = player_a;
}

static void set_player(PLAYER * player)
{
    printf("<< プレイヤータイプを選択してください\n");
    printf("   %d: 人間\n", HUMAN);
    printf("   %d: AI（C言語）\n", C_AGENT);
    printf("   %d: AI（Python）\n", PYTHON_AGENT);
    printf(">> ");
    scanf("%d", &player->player_type);
    printf("<< %d が選択されました\n", player->player_type);
    printf("\n");

    printf("<< プレイヤー名を設定してください\n");
    printf(">> ");
    scanf("%s", player->name);
    printf("<< %s が入力されました\n", player->name);
    printf("\n");
    player->next_choices[0] = -1;	/* 番兵で初期化 */
}

int main(void)
{
    PLAYER player_a;            /* 先攻 */
    PLAYER player_b;            /* 後攻 */
    PLAYER *winner = NULL;

    player_a.stone = BLACK;
    player_b.stone = WHITE;

    printf("■ 初期設定\n");
    printf("== 先攻 ==\n");
    set_player(&player_a);
    printf("== 後攻 ==\n");
    set_player(&player_b);

    proceed_game(&player_a, &player_b, winner);

    return 0;
}
