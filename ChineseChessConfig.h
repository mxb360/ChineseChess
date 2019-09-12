#ifndef CHINESECHESS_H
#define CHINESECHESS_H

#pragma execution_character_set("utf-8")

#define CHESS_SIZE 46
#define GRID_SIZE  60

#define OX         (CHESS_SIZE/2 + 10)
#define OY         (CHESS_SIZE/2 + 10)

// 将棋子行列坐标转换为像素坐标
#define L_X(x) (OX + (x) * GRID_SIZE)
#define C_Y(y) (OY + (y) * GRID_SIZE)
#define P_X(x) (L_X(x) - CHESS_SIZE/2)
#define P_Y(y) (C_Y(y) - CHESS_SIZE/2)
#define X_L(x) ((x - OX) / GRID_SIZE)
#define Y_C(y) ((y - OY) / GRID_SIZE)

#define RED   0
#define GREEN 1

#define SHUAI 0
#define JU    1
#define PAO   2
#define MA    3
#define XIANG 4
#define SHI   5
#define BING  6

#endif
