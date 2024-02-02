#include <stdio.h>
#include <stdbool.h>
#include "board.h"

BoardI directions[8] = { 10, 11, 1, -9, -10, -11, -1, 9 };

char count_stone(const BoardState stone, const Board * board)
{
    char x, y;
    char count = 0;
    for (x = 1; x < BOARD_HEIGHT - 1; x++) {
        for (y = 1; y < BOARD_WIDTH - 1; y++) {
            if (board->array[GET_BOARD_I(x, y)] == stone) {
                count++;
            }
        }
    }
    return count;
}

void init_board(Board * board)
{
    char x, y;

    for (x = 0; x < BOARD_HEIGHT; x++) {
        for (y = 0; y < BOARD_WIDTH; y++) {
            if (x == 0 || x == BOARD_HEIGHT - 1 || y == 0
                || y == BOARD_WIDTH - 1) {
                board->array[GET_BOARD_I(x, y)] = WALL;
            } else {
                board->array[GET_BOARD_I(x, y)] = EMPTY;
            }
        }
    }

    board->array[GET_BOARD_I(BOARD_HEIGHT / 2, BOARD_WIDTH / 2)] = WHITE;
    board->array[GET_BOARD_I
                 ((BOARD_HEIGHT / 2) - 1, (BOARD_WIDTH / 2) - 1)] = WHITE;
    board->array[GET_BOARD_I((BOARD_HEIGHT / 2) - 1, (BOARD_WIDTH / 2))] =
        BLACK;
    board->array[GET_BOARD_I((BOARD_HEIGHT / 2), (BOARD_WIDTH / 2) - 1)] =
        BLACK;
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

    printf(" ");
    for (y = 1; y < BOARD_WIDTH - 1; y++) {
        printf(" %d", y - 1);
    }
    printf("\n");

    for (x = 1; x < BOARD_HEIGHT - 1; x++) {
        printf("%d|", x - 1);
        for (y = 1; y < BOARD_WIDTH - 1; y++) {
            print_board_state_icon(board->array[GET_BOARD_I(x, y)]);
            printf("|");
        }
        printf("\n");
    }
}

char count_reverse_in_the_dir(const BoardI board_i, const BoardState stone,
                              const char dir, const Board * board)
{
    char count = 0;
    BoardI i = board_i;
    BoardState current;
    while (1) {
        i += dir;
        current = board->array[i];
        if (current == EMPTY || current == WALL) {
            return 0;
        } else if (current == stone) {
            return count;
        } else {
            count++;
        }
    }
}

bool can_reverse(const BoardI board_i, const BoardState stone,
                 const Board * board)
{
    char dir_i;
    for (dir_i = 0; dir_i < 8; dir_i++) {
        if (count_reverse_in_the_dir
            (board_i, stone, directions[dir_i], board) != false) {
            return true;
        }
    }
    return false;
}

void reverse_stone(const BoardI board_i, const BoardState stone,
                   Board * board)
{
    char i, dir_i, dir;
    for (dir_i = 0; dir_i < 8; dir_i++) {
        dir = directions[dir_i];
        if (count_reverse_in_the_dir(board_i, stone, dir, board) != false) {
            i = board_i + dir;
            while (board->array[i] != stone) {
                board->array[i] = stone;
                i += dir;
            }
        }
    }
}

bool can_put_stone(const BoardI board_i, const BoardState stone,
                   const Board * board)
{
    if (board->array[board_i] != EMPTY) {
        return false;
    } else if (can_reverse(board_i, stone, board) == false) {
        return false;
    }
    return true;
}

void put_stone(const BoardI board_i, const BoardState stone, Board * board)
{
    board->array[board_i] = stone;
    reverse_stone(board_i, stone, board);
}

char count_next_choices(const BoardI *next_choices)
{
    char i = 0;
    while (next_choices[i] != -1) {
        i++;
    }
    return i;
}

char push_next_choices(BoardI *next_choices, const BoardI board_i)
{
    char i = count_next_choices(next_choices);
    next_choices[i] = board_i;
    next_choices[i + 1] = -1;
    return i + 1;
}

char get_next_choices(BoardI *next_choices, const BoardState stone, const Board * board)
{
    char x, y, count = 0;
    for (x = 1; x < BOARD_HEIGHT - 1; x++) {
        for (y = 1; y < BOARD_WIDTH - 1; y++) {
            if (can_put_stone(GET_BOARD_I(x, y), stone, board) != false) {
                push_next_choices(next_choices, GET_BOARD_I(x, y));
            }
        }
    }
    return count_next_choices(next_choices);
}
