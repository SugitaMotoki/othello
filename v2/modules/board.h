#ifndef _INCLUDE_BOARD_
#define _INCLUDE_BOARD_

#include <stdbool.h>

/* オセロのルール上の盤の大きさ */
#define FIELD_SIZE (8)

/* 内部データ（Board.array）の上での盤の横幅 */
#define BOARD_WIDTH ((FIELD_SIZE) + 2)
/* 内部データ（Board.array）の上での盤の縦幅 */
#define BOARD_HEIGHT ((FIELD_SIZE) + 2)

/* BoardIの値からX座標を割り出すマクロ関数 */
#define GET_BOARD_X(I) ((I) / (BOARD_HEIGHT))
/* BoardIの値からY座標を割り出すマクロ関数 */
#define GET_BOARD_Y(I) ((I) % (BOARD_HEIGHT))
/* X座標とY座標からBoardIの値を割り出すマクロ関数 */
#define GET_BOARD_I(X,Y) ((X) * (BOARD_HEIGHT) + (Y))

/* 盤上の各マスがとりうる状態 */
typedef enum board_state {
    EMPTY,
    WHITE,
    BLACK,
    WALL,
} BoardState;

/* 盤全体の状態 */
typedef struct board {
    BoardState array[BOARD_WIDTH * BOARD_HEIGHT];
} Board;

/* Board.arrayのインデックス（マスの位置を表す） */
typedef char BoardI;

/* 隣のマスを参照する際にBoardIに足す数字（8方位） */
extern BoardI directions[8];



/* 盤上の石を数える関数 */
extern char count_stone(const BoardState stone, const Board * board);

/* 盤をゲーム開始時の（2個ずつ設置された）状態に初期化する関数 */
extern void init_board(Board * board);

/* 各BoardStateに対応する記号を出力する関数 */
extern void print_board_state_icon(const BoardState state);

/* 盤の状態を出力する関数 */
extern void print_board(const Board * board);

/* 1方向に対し、ひっくり返せる相手の石の数を数える関数 */
extern char count_reverse_in_the_dir(const BoardI board_i,
                                     const BoardState stone,
                                     const char dir, const Board * board);

/* 相手の石をひっくり返すことができるかを判定する関数 */
extern bool can_reverse(const BoardI board_i, const BoardState stone,
                        const Board * board);

/* 相手の石をひっくり返す関数 */
extern void reverse_stone(const BoardI board_i, const BoardState stone,
                          Board * board);

/* 石を設置することができるかを判定する関数 */
extern bool can_put_stone(const BoardI board_i, const BoardState stone,
                          const Board * board);

/* 石を設置する関数 */
extern void put_stone(const BoardI board_i, const BoardState stone,
                      Board * board);

/* 合法手の数を数える関数 */
extern char count_next_choices(const BoardI *next_choices);

/* 合法手一覧の配列に値を加える関数 */
extern char push_next_choices(BoardI *next_choices, const BoardI board_i);

/* 合法手一覧を返す関数 */
extern char get_next_choices(BoardI *next_choices, const BoardState stone, const Board * board);

#endif
