#include <stdio.h>
#include <stdbool.h>

#define ADDITIONAL_PRINT true
#define MAX_LENGTH_OF_PLAYER_NAME 30
#define SIZE (8)                /* オセロなので8*8 */
#define WIDTH (SIZE + 2)
#define HEIGHT (SIZE + 2)

typedef enum board_state {
    white,                      /* 白石 */
    black,                      /* 黒石 */
    empty,                      /* 空白 */
    wall                        /* 壁 */
} BOARD_STATE;

typedef enum player {
    player_a,                   /* 白 */
    player_b,                   /* 黒 */
} PLAYER;

typedef enum game_state {
    ready,                      /* 開始前 */
    playing,                    /* プレイ中 */
    skipped,                    /* 片方がスキップした状態 */
    finished,                   /* ゲーム終了後 */
} GAME_STATE;

/* 盤 */
BOARD_STATE board[WIDTH][HEIGHT];

/* プレイヤー名 */
char *player_names[2] = {
    "Player_A",
    "Player_B",
};

int directions[8][2] = {
    { 1, 0 },
    { 1, 1 },
    { 0, 1 },
    { -1, 1 },
    { -1, 0 },
    { -1, -1 },
    { 0, -1 },
    { 1, -1 },
};

/* 盤を初期化する */
void init_board(void)
{
    int i, j;

    /* 壁と空白で埋める */
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1) {
                board[i][j] = wall;
            } else if (j == 0 || j == WIDTH - 1) {
                board[i][j] = wall;
            } else {
                board[i][j] = empty;
            }
        }
    }

    /* 初期配置 */
    board[HEIGHT / 2][WIDTH / 2] = black;
    board[HEIGHT / 2 - 1][WIDTH / 2 - 1] = black;
    board[HEIGHT / 2 - 1][WIDTH / 2] = white;
    board[HEIGHT / 2][WIDTH / 2 - 1] = white;
}

void print_board_state_icon(BOARD_STATE state)
{
    switch (state) {
    case white:
        printf("○");
        break;
    case black:
        printf("●");
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

/* 盤の状態を出力する */
void print_board(void)
{
    int i, j;

    printf("  j\ni ");
    for (j = 1; j < WIDTH - 1; j++) {
        printf(" %d", j);
    }
    printf("\n");

    for (i = 1; i < HEIGHT - 1; i++) {
        printf(" %d|", i);
        for (j = 1; j < WIDTH - 1; j++) {
            print_board_state_icon(board[i][j]);
            printf("|");
        }
        printf("\n");
    }
}

int count_reversible_stone(const int i, const int j, BOARD_STATE stone,
                           const int direction_index)
{
    BOARD_STATE current;
    int count = 0;
    int x = i;
    int y = j;
    int dif_x = directions[direction_index][0];
    int dif_y = directions[direction_index][1];

    while (1) {
        x = x + dif_x;
        y = y + dif_y;
        current = board[x][y];
        if (current == empty || current == wall) {
            return 0;
        } else if (current == stone) {
            return count;
        } else {
            count++;
        }
    }
}

/* 指定された位置に石を置けるか判定する */
bool can_put_stone(int i, int j, BOARD_STATE stone)
{
    int d;
    if (board[i][j] != empty) {
        return false;
    }
    for (d = 0; d < 8; d++) {
        if (count_reversible_stone(i, j, stone, d) != 0) {
            return true;
        }
    }
    return false;
}

/* 石を裏返す */
void reverse_stone(const int i, const int j, BOARD_STATE stone,
                   const int direction_index)
{
    int dif_x = directions[direction_index][0];
    int dif_y = directions[direction_index][1];
    int x = i + dif_x;
    int y = j + dif_y;

    while (board[x][y] != stone) {
        board[x][y] = stone;
        x = x + dif_x;
        y = y + dif_y;
    }
}

/* 石を設置する */
void put_stone(int i, int j, BOARD_STATE stone)
{
    int d;
    board[i][j] = stone;
    for (d = 0; d < 8; d++) {
        if (count_reversible_stone(i, j, stone, d) != 0) {
            reverse_stone(i, j, stone, d);
        }
    }
}

/* ゲームが終了したかを判定する*/
bool is_finished()
{
    return false;
}

/* 次に置く座標を標準入力から受け取る*/
void get_location_from_stdin(int *i, int *j)
{
    if (ADDITIONAL_PRINT) {
        printf("Please input i j: ");
    }
    scanf("%d%d", i, j);
}

PLAYER change_player(PLAYER player)
{
    return 1 - player;
}

BOARD_STATE get_board_state_of(PLAYER player)
{
    if (player == player_a) {
        return white;
    } else {
        return black;
    }
}

/* CLIでプレイする場合に必要な情報を出力する */
void print_info(PLAYER player)
{
    print_board();
    printf("%s's turn.\n", player_names[player]);
    printf("stone color: [");
    print_board_state_icon(get_board_state_of(player));
    printf("]\n");
}

/* ゲームを進行する */
int proceed_game()
{
    int input_i, input_j;
    PLAYER player = player_b; /* 先攻は黒 */

    init_board();
    printf("Game start!\n\n");

    while (is_finished() == false) {
        if (ADDITIONAL_PRINT == true) {
            print_info(player);
        }

        get_location_from_stdin(&input_i, &input_j);

        if (can_put_stone(input_i, input_j, get_board_state_of(player))) {
            put_stone(input_i, input_j, get_board_state_of(player));
            player = change_player(player);
        } else {
            printf("You can't put the location.\n");
        }
    }

    printf("Finish!\n");

    return player_a;
}

int main(void)
{
    int winner;

    winner = proceed_game();
    if (winner == -1) {
        printf("Draw.");
    }
    printf("The winner is ... %s !!!\n", player_names[winner]);
    return 0;
}
