#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "../modules/board.h"

struct timeval tv;

static void create_board(char *input, Board * board)
{
    char x, y, i;
    for (x = 0; x < BOARD_HEIGHT; x++) {
        for (y = 0; y < BOARD_WIDTH; y++) {
            if (x == 0 || x == BOARD_HEIGHT - 1 || y == 0
                || y == BOARD_WIDTH - 1) {
                board->array[GET_BOARD_I(x, y)] = WALL;
            } else {
                board->array[GET_BOARD_I(x, y)] = input[i] - '0';
                i++;
            }
        }
    }
}

// int main(void)
int main(int argc, char *argv[])
{
    Board board;
    BoardState stone;
    char next_choices[16] = { -1 };
    char count_next = 0;
    // char input[66];
    char x, y;
    int random_i, next_i;

    next_choices[0] = -1;
    // scanf("%s", input);
    create_board(argv[1], &board);
    stone = argv[1][65] - '0';
    count_next = get_next_choices(next_choices, stone, &board);

    if (count_next == 0) {
        printf("{\"x\":8,\"y\":0}");
    } else {
        gettimeofday(&tv, NULL);
        srand(tv.tv_sec + tv.tv_usec);
        random_i = (int) (rand() * ((count_next - 1) + 1.0) /
                          (RAND_MAX + 1.0));
        next_i = next_choices[random_i];
        printf("{\"x\":%d,\"y\":%d}", GET_BOARD_X(next_i) - 1,
               GET_BOARD_Y(next_i) - 1);
    }

    return 0;
}
