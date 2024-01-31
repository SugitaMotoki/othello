#include <stdio.h>
#include "modules/board.h"

int main(void)
{
    char black, white;
    Board board;

    init_board(&board);
    print_board(&board);
    count_stone(&black, &white, &board);
    printf("b: %d, w: %d\n", black, white);

    put_stone(GET_BOARD_I(2 + 1, 3 + 1), BLACK, &board);
    print_board(&board);
    count_stone(&black, &white, &board);
    printf("b: %d, w: %d\n", black, white);

    put_stone(GET_BOARD_I(2 + 1, 4 + 1), WHITE, &board);
    print_board(&board);
    count_stone(&black, &white, &board);
    printf("b: %d, w: %d\n", black, white);

    put_stone(GET_BOARD_I(2 + 1, 5 + 1), BLACK, &board);
    print_board(&board);
    count_stone(&black, &white, &board);
    printf("b: %d, w: %d\n", black, white);

    if (can_put_stone(GET_BOARD_I(2 + 1, 2 + 1), BLACK, &board) != false) {
        printf("true\n");
        print_board(&board);
        count_stone(&black, &white, &board);
        printf("b: %d, w: %d\n", black, white);
    } else {
        printf("false\n");
    }

    return 0;
}
