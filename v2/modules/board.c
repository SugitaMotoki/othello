#include <stdio.h>
#include "board.h"

BoardI directions[8] = { 10, 11, 1, -9, -10, -11, -1, 9 };

void count_stone(char *black, char *white, const Board * board)
{
    char x, y;
    *black = 0;
    *white = 0;
    for (x = 1; x < BOARD_HEIGHT - 1; x++) {
        for (y = 1; y < BOARD_WIDTH - 1; y++) {
            switch (board->array[GET_BOARD_I(x, y)]) {
            case BLACK:
                (*black)++;
                break;
            case WHITE:
                (*white)++;
                break;
            default:
                continue;
                break;
            }
        }
    }
}

void init_board(Board * board) {
    int x, y;

    for (x = 0; x < BOARD_HEIGHT; x++) {
        for (y = 0; y < BOARD_WIDTH; y++) {
            if (x == 0 || x == BOARD_HEIGHT - 1 || y == 0 || y == BOARD_WIDTH - 1) {
                board->array[GET_BOARD_I(x, y)] = WALL;
            } else {
                board->array[GET_BOARD_I(x, y)] = EMPTY;
            }
        }
    }

    board->array[GET_BOARD_I(BOARD_HEIGHT / 2, BOARD_WIDTH / 2)] = WHITE;
    board->array[GET_BOARD_I((BOARD_HEIGHT / 2) - 1, (BOARD_WIDTH / 2) - 1)] = WHITE;
    board->array[GET_BOARD_I((BOARD_HEIGHT / 2) - 1, (BOARD_WIDTH / 2))] = BLACK;
    board->array[GET_BOARD_I((BOARD_HEIGHT / 2), (BOARD_WIDTH / 2) - 1)] = BLACK;
}

void print_board_state_icon(const BoardState state)
{
    switch (state) {
    case EMPTY:
        printf(" ");
        break;
    case BLACK:
        printf("●");
        break;
    case WHITE:
        printf("○");
        break;
    case WALL:
        printf("w");
        break;
    default:
        printf("!");
        break;
    }
}

void print_board(const Board * board)
{
    char x, y;

    printf("  y\nx ");
    for (y = 1; y < BOARD_WIDTH - 1; y++) {
        printf(" %d", y - 1);
    }
    printf("\n");

    for (x = 1; x < BOARD_HEIGHT - 1; x++) {
        printf(" %d|", x - 1);
        for (y = 1; y < BOARD_WIDTH - 1; y++) {
            print_board_state_icon(board->array[GET_BOARD_I(x, y)]);
            printf("|");
        }
        printf("\n");
    }
}
