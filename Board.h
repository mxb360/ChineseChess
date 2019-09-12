#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsView>
#include <QList>
#include <QMediaPlayer>

#include "Chess.h"
#include "ChineseChessConfig.h"
#include "ChessManual.h"

class Board : public QWidget
{
public:
    Board();
    ~Board();
    void  draw_board(QPainter &);
    void  init_board_by_manual(ChessManual *manual);
    Chess *get_chess_by_pos(int x, int y);
    void  remove_chess(Chess *chess);
    int   get_chess_count(int x1, int y1, int x2, int y2);
    int   chess_can_move_to(Chess *chess, int x, int y);
    int   move_chess_to_pos(int x1, int y1, int x2, int y2);
    void  chess_moving();
    int   is_game_over();
    void  game_over();
    void  restart();
    bool  close_event(void);

private:
    QPoint get_chess_pos_by_mouse_pos(int x, int y)
    {
        QPoint point = QPoint(-1, -1);
        int lx = X_L(x);
        int cy = Y_C(y);

        if ((x - L_X(lx))*(x - L_X(lx)) + (y - L_X(cy))*(y - L_X(cy)) < CHESS_SIZE*CHESS_SIZE / 4) {
            point.setX(lx), point.setY(cy);
            return point;
        }
        lx += 1;
        if ((x - L_X(lx))*(x - L_X(lx)) + (y - L_X(cy))*(y - L_X(cy)) < CHESS_SIZE*CHESS_SIZE / 4) {
            point.setX(lx), point.setY(cy);
            return point;
        }
        cy += 1;
        if ((x - L_X(lx))*(x - L_X(lx)) + (y - L_X(cy))*(y - L_X(cy)) < CHESS_SIZE*CHESS_SIZE / 4) {
            point.setX(lx), point.setY(cy);
            return point;
        }
        lx -= 1;
        if ((x - L_X(lx))*(x - L_X(lx)) + (y - L_X(cy))*(y - L_X(cy)) < CHESS_SIZE*CHESS_SIZE / 4) {
            point.setX(lx), point.setY(cy);
            return point;
        }
        return point;
    }

    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *);

    int grid_size;          // 棋格大小
    int ox, oy;             // 棋子原点坐标
    Chess *choice;          // 选中的棋子
    int current_chess_color;                 // 落子方
    QPoint pt_mov_to = QPoint(-1, -1);
    QList<Chess> chesses;
    int is_chess_moving = 0;
    QMediaPlayer *player;
};

#endif // BOARD_H
