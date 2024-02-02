#include <stdio.h>
#include <stdlib.h>
#include "modules/board.h"

#define DEBUG_PRINT (false)

typedef struct player {
    BoardState stone;
    char player_name[50];
    char agent_name[50];
    int number_of_wins;
    bool is_passed;
} Player;

Player player_a = {
    BLACK,
    "random01",
    "random",
    0,
    false,
};

Player player_b = {
    WHITE,
    "random02",
    "random",
    0,
    false,
};

static void get_board_str(char *board_str, const Board * board)
{
    int x, y, i = 0;
    for (x = 1; x < BOARD_HEIGHT - 1; x++) {
        for (y = 1; y < BOARD_WIDTH - 1; y++) {
            board_str[i] = board->array[GET_BOARD_I(x, y)] + '0';
            i++;
        }
    }
}

static void get_position(char *x, char *y, Player * player,
                         const Board * board)
{
    FILE *fp;
    char board_str[66] = "";
    char command[256];
    char move_json[16] = "";

    get_board_str(board_str, board);
    snprintf(command, 256, "./agents/%s.out %s,%d",
             player->agent_name, board_str, player->stone);

    fp = popen(command, "r");
    if (fp == NULL) {
        perror("popen failed");
        exit(-1);
    }
    fgets(move_json, 16, fp);
    pclose(fp);

    /* {"x":?,"y":?} */
    *x = move_json[5] - '0';
    *y = move_json[11] - '0';
}

static bool is_finished(const Board * board)
{
    if (player_a.is_passed != false && player_b.is_passed != false) {
        return true;
    } else if (count_stone(EMPTY, board) == 0) {
        return true;
    }
    return false;
}

static Player *proceed_game()
{
    Board board;
    Player *attacker;
    int turn_count = 0;
    char x, y, i;
    char a_stone, b_stone;
    bool put_result;

    init_board(&board);
    player_a.is_passed = false;
    player_b.is_passed = false;

    while (is_finished(&board) == false) {
        attacker = (turn_count % 2 != 0) ? &player_b : &player_a;

        if (DEBUG_PRINT != false) {
            print_board(&board);
            printf("%s\n", attacker->player_name);
        }

        get_position(&x, &y, attacker, &board);
        i = GET_BOARD_I(x + 1, y + 1);
        if (x == 8 && y == 0) {
            turn_count++;
            attacker->is_passed = true;
            continue;
        } else if (can_put_stone(i, attacker->stone, &board) == false) {
            printf("%s: (%d,%d)には置けません\n",
                   attacker->player_name, x, y);
            continue;
        }
        put_stone(i, attacker->stone, &board);
        attacker->is_passed = false;
        turn_count++;
    }
    a_stone = count_stone(player_a.stone, &board);
    b_stone = count_stone(player_b.stone, &board);
    print_board(&board);
    if (a_stone > b_stone) {
        return &player_a;
    } else if (a_stone < b_stone) {
        return &player_b;
    } else {
        return NULL;
    }
}

int main(void)
{
    int game_count = 1;
    int max_game_count;
    Player *winner;

    printf("回数: ");
    scanf("%d", &max_game_count);

    while (game_count <= max_game_count) {
        printf("== %d ==\n", game_count);
        winner = proceed_game();
        if (winner != NULL) {
            printf("%sの勝ち\n", winner->player_name);
            winner->number_of_wins++;
        } else {
            printf("引き分け\n");
        }
        game_count++;
    }

    printf("%s: %d\n", player_a.player_name, player_a.number_of_wins);
    printf("%s: %d\n", player_b.player_name, player_b.number_of_wins);

    return 0;
}
