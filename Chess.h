#ifndef CHESS_H
#define CHESS_H

#include <QPainter>
#include "ChineseChessConfig.h"

class Chess
{
public:
    Chess(int name, int color, int x, int y);
    void draw(QPainter &painter);      // 绘制棋子
    int  can_move_to(int x, int y);    // 棋子是否可移动（未考虑其他棋子的影响）

    int name;            // 棋子名
    int x, y;            // 棋子在棋盘上的坐标
    int color;           // 棋子颜色
    int is_choice;       // 棋子是否被选中
    int is_moving;       // 棋子是否正在移动（动画）
    QString str_name;    // 棋子的字符描述
    QString str_color;   // 棋子字体颜色

private:
    QColor _color;
    QBrush _brush = QBrush(Qt::darkYellow, Qt::SolidPattern);
    QPen   _pen   = QPen(Qt::yellow, 2);
    QFont  _font  = QFont("宋体", 20, 20);
    void   _draw(QPainter &painter, int x, int y)
    {
        painter.setBrush(_brush);
        painter.setPen(_pen);
        painter.setFont(_font);
        _pen.setColor(_color);
        painter.setPen(_pen);
        painter.drawEllipse(x - CHESS_SIZE / 2, y - CHESS_SIZE / 2, CHESS_SIZE, CHESS_SIZE);
        // 如果改动了字体大小，以下参数的值可能要重调
        painter.drawText(x - CHESS_SIZE / 2 + 7, y + GRID_SIZE / 4 - 3, str_name);
    }

};

#endif // CHESS_H
