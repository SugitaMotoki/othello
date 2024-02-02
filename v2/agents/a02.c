#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "../modules/board.h"

#define MAX_DEPTH (3)
#define ANOTHER_STONE(S) ((BLACK) + (WHITE) - (S))

struct timeval tv;

typedef struct node {
    Board board;
    BoardState stone;
    BoardI board_i;
    int point;
    char depth;
    struct node **children;
    struct node *best;
} Node;

Node root;

char position_score[BOARD_WIDTH * BOARD_HEIGHT] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 30, -12, 0, -1, -1, 0, -12, 30, 0,
    0, -12, -15, -3, -3, -3, -3, -15, -12, 0,
    0, 0, -3, 0, -1, -1, 0, -3, 0, 0,
    0, -1, -3, -1, -1, -1, -1, -3, -1, 0,
    0, -1, -3, -1, -1, -1, -1, -3, -1, 0,
    0, 0, -3, 0, -1, -1, 0, -3, 0, 0,
    0, -12, -15, -3, -3, -3, -3, -15, -12, 0,
    0, 30, -12, 0, -1, -1, 0, -12, 30, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static int evaluate_terminal(Node * node)
{
    int point = 0;
    char x, y, i;
    char my_stone = 0;
    char enemy_stone = 0;

    for (x = 1; x < BOARD_HEIGHT - 1; x++) {
        for (y = 1; y < BOARD_WIDTH - 1; y++) {
            i = GET_BOARD_I(x, y);
            if (node->board.array[i] == node->stone) {
                point += position_score[i];
                my_stone++;
            } else if (node->board.array[i] != EMPTY) {
                point -= position_score[i];
                enemy_stone++;
            }
        }
    }

    if (enemy_stone == 0) {
        point += 100000;
    } else if (my_stone == 0) {
        point -= 100000;
    } else if (my_stone + enemy_stone == FIELD_SIZE * FIELD_SIZE) {
        point += (my_stone - enemy_stone) * 1000;
    }
    return point;
}

static Node *create_child_node(const Node * parent, const BoardI board_i)
{
    Node *child = malloc(sizeof(Node));
    child->stone = ANOTHER_STONE(parent->stone);
    child->depth = parent->depth + 1;
    child->board = parent->board;
    child->board_i = board_i;
    if (board_i != -1) {
        put_stone(child->board_i, parent->stone, &child->board);
    }
    return child;
}

static int negamax(Node * node, const bool is_pass)
{
    char i, number_of_child;
    BoardI next_choices[FIELD_SIZE * FIELD_SIZE] = { -1 };
    int child_point, best_point;

    if (node->depth == MAX_DEPTH) {
        return evaluate_terminal(node);
    }

    number_of_child =
        get_next_choices(next_choices, node->stone, &node->board);

    if (number_of_child == 0) {
        if (node->depth == 0) {
            node->best = NULL;
            return 0;
        } else {
            if (is_pass != false) {
                return evaluate_terminal(node);
            }
            node->children = malloc(sizeof(Node *));
            node->children[0] = create_child_node(node, -1);
            best_point = -negamax(node->children[0], true);
        }
    } else {
        node->children = malloc(sizeof(Node *) * number_of_child);
        for (i = 0; i < number_of_child; i++) {
            node->children[i] = create_child_node(node, next_choices[i]);
            child_point = -negamax(node->children[i], false);
            if (i == 0 || child_point > best_point) {
                best_point = node->children[i]->point;
                node->best = node->children[i];
            }
        }
    }

    return best_point;
}

static void init_root_node(char *input_str)
{
    char x, y, i;
    for (x = 0; x < BOARD_HEIGHT; x++) {
        for (y = 0; y < BOARD_WIDTH; y++) {
            if (x == 0 || x == BOARD_HEIGHT - 1 || y == 0
                || y == BOARD_WIDTH - 1) {
                root.board.array[GET_BOARD_I(x, y)] = WALL;
            } else {
                root.board.array[GET_BOARD_I(x, y)] = input_str[i] - '0';
                i++;
            }
        }
    }

    root.stone = input_str[65] - '0';
    root.depth = 0;
}

int main(int argc, char *argv[])
{
    char *input_str;

    if (argc == 2) {
        input_str = argv[1];
    } else {
        input_str = malloc(sizeof(char) * 66);
        scanf("%s", input_str);
    }

    init_root_node(input_str);
    negamax(&root, false);

    if (root.best == NULL) {
        printf("{\"x\":8,\"y\":0}");
    } else {
        printf("{\"x\":%d,\"y\":%d}", GET_BOARD_X(root.best->board_i) - 1,
               GET_BOARD_Y(root.best->board_i) - 1);
    }

    return 0;
}
