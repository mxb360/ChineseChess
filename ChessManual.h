#ifndef CHESS_MANUAL_H
#define CHESS_MANUAL_H

#include "ChineseChessConfig.h"
#include <QVector>
#include <QPair>
#include <QPoint>

#define MC(chess, color) (chess)*10+(color)

class ChessManual
{
public:
    ChessManual();
    ~ChessManual();
    int chesses[10][9];
private:
    QVector<QPair<QPoint, QPoint>> manual;
};

#endif  // !CHESS_MANUAL_H


