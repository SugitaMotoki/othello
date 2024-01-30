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
    return 0;
}
