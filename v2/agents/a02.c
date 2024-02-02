#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "../modules/board.h"

struct timeval tv;

int main(int argc, char *argv[])
{
    Board board;
    init_board(&board);
    print_board(&board);
    return 0;
}
