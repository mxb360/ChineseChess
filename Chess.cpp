#include "Chess.h"

// 棋子文字，用于在界面上显示
const QString _chess_name[7][2] = { 
    {"帥", "將"}, {"車", "車"}, {"炮", "炮"}, {"馬", "馬"}, 
    {"象", "相"}, {"士", "士"}, {"兵", "卒"},
};


Chess::Chess(int name, int color, int x, int y)
{
    this->name = name;
    this->x = x;
    this->y = y;
    this->color = color;
    this->str_name = QString(_chess_name[name][color]);
    is_choice = 0;
    is_moving = 0;

    if (color) {
        _color = Qt::green;
        str_color = QString("绿");
    } else {
        _color = Qt::red;
        str_color = QString("红");
    }
}

// 绘制棋子
void Chess::draw(QPainter &painter)
{
    if (is_choice) {
        if (color == GREEN)
            _brush.setColor(Qt::darkGreen);
        else
            _brush.setColor(Qt::darkRed);
    }
    else
        _brush.setColor(Qt::darkYellow);
    is_moving ? _draw(painter, x, y) : _draw(painter, L_X(x), C_Y(y));
}

// 描述棋子的行走规则（未考虑其他棋子的影响）
int Chess::can_move_to(int x_, int y_)
{
    switch (name) {
    case JU:                    // 車
    case PAO:                   // 炮
        return x == x_ || y == y_;
    case MA:                    // 馬
        return (x == x_ - 1 && y == y_ - 2 || x == x_ + 1 && y == y_ - 2 ||
            x == x_ + 2 && y == y_ - 1 || x == x_ + 2 && y == y_ + 1 ||
            x == x_ + 1 && y == y_ + 2 || x == x_ - 1 && y == y_ + 2 ||
            x == x_ - 2 && y == y_ + 1 || x == x_ - 2 && y == y_ - 1);
    case XIANG:                 // 象，相
        return (color == RED && y_ > 4 || color == GREEN && y_ <= 4) &&
               (x == x_ - 2 && y == y_ - 2 || x == x_ + 2 && y == y_ - 2 ||
                x == x_ + 2 && y == y_ + 2 || x == x_ - 2 && y == y_ + 2);
    case SHI:                   // 士
        return x_ > 2 && x_ < 6 && (color == RED && y_ > 6 || color == GREEN && y_ < 3) &&
              (x == x_ + 1 && y == y_ + 1 || x == x_ - 1 && y == y_ + 1 ||
               x == x_ + 1 && y == y_ - 1 || x == x_ - 1 && y == y_ - 1);
    case SHUAI:                 // 帥，將
        return x_ > 2 && x_ < 6 && (color == RED && y_ > 6 || color == GREEN && y_ < 3) &&
            (x == x_ && y == y_ + 1 || x == x_ && y == y_ - 1 ||
                x == x_ + 1 && y == y_ || x == x_ - 1 && y == y_);
    case BING:
        if (color == RED)       // 兵
            return x == x_ && y == y_ + 1 || y <= 4 && y == y_ && (x == x_ + 1 || x_ == x + 1);
        else                    // 卒
            return x == x_ && y == y_ - 1 || y > 4 && y == y_ && (x == x_ + 1 || x_ == x + 1);
    }
    return 0;
}

