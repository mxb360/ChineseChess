#include "Board.h"
#include <QDebug>
#include <qevent.h>
#include <QMessageBox>

static void _draw_cross(int x, int y, QPainter &painter);

// 棋盘类
Board::Board()
{
    player = new QMediaPlayer;
    
    player->setMedia(QUrl::fromLocalFile("Resources/audio_room_back.mp3"));
    player->setVolume(30);
    player->play();
    restart();
}

Board::~Board()
{
    delete player;
}

// 获得(x, y)处的棋子对象指针
Chess *Board::get_chess_by_pos(int x, int y)
{
    for (int i = 0; i < chesses.length(); i++)
        if (chesses[i].x == x && chesses[i].y == y)
            return &chesses[i];
    return nullptr;
}

// 获得两点之间存在的棋子个数（不包括端点）
int Board::get_chess_count(int x1, int y1, int x2, int y2)
{ 
    int i, count = 0;
    int abs_x = abs(x2 - x1);
    int abs_y = abs(y2 - y1);
    int dx = x2 - x1 ? (x2 - x1) / abs_x : 0;
    int dy = y2 - y1 ? (y2 - y1) / abs_y : 0;

    if (!dx || !dy) { // 横排，纵排，针对车，炮
        for (i = 1; i < abs_x || i < abs_y; i++)
            if (get_chess_by_pos(x1 + i * dx, y1 + i * dy))
                count++;
    } else if (abs_x == abs_y)  // 对角，针对相，象
        count = !!get_chess_by_pos((x1 + x2) / 2, (y1 + y2) / 2) != NULL;
    else if (abs_x == 1)            // 针对马
        count = !!get_chess_by_pos(x1, (y1 + y2) / 2);
    else if (abs_y == 1)            // 针对马
        count = !!get_chess_by_pos((x1 + x2) / 2, y1);
    return count;
}

// 判断
int Board::chess_can_move_to(Chess *chess, int x, int y)
{
    if (!chess || (chess->name != SHUAI && !chess->can_move_to(x, y)))
        return 0;

    int count = get_chess_count(chess->x, chess->y, x, y);
    Chess *_chess = get_chess_by_pos(x, y);
    
    switch (chess->name) {
    case SHI: case BING: 
        return 1;
    case JU: case MA: case XIANG: 
        return count == 0;
    case PAO: 
        return (!count && !_chess) || (count == 1 && _chess);
    case SHUAI:
        return (chess->x == x && _chess && _chess->name == SHUAI && count == 0) ||
                chess->can_move_to(x, y);
    }

    return 0;
}

int Board::move_chess_to_pos(int x1, int y1, int x2, int y2)
{
    Chess *chess = get_chess_by_pos(x1, y1);
    if (chess_can_move_to(chess, x2, y2)) {
        choice = chess;
        pt_mov_to = QPoint(x2, y2);
        chess_moving();
        choice->is_choice = 0;
        choice = nullptr;
        pt_mov_to.setX(-1);
        return 1;
    }
    return 0;
}

// 绘制棋盘
void Board::draw_board(QPainter &painter)
{
    QPen pen = QPen(Qt::black, 5);
    painter.setPen(pen);
    painter.drawRect(OX - 7, OY - 7, 8 * GRID_SIZE + 14, 9 * GRID_SIZE + 14);

    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawRect(ox, oy, 8 * grid_size, 9 * grid_size);

    for (int i = 1; i < 9; i++)
        painter.drawLine(L_X(i), C_Y(0), L_X(i), C_Y(4));
    for (int i = 1; i < 9; i++)
        painter.drawLine(L_X(i), C_Y(5), L_X(i), C_Y(9));
    for (int i = 1; i < 10; i++)
        painter.drawLine(L_X(0), C_Y(i), L_X(8), C_Y(i));

    painter.drawLine(L_X(3), C_Y(0), L_X(5), C_Y(2));
    painter.drawLine(L_X(5), C_Y(0), L_X(3), C_Y(2));
    painter.drawLine(L_X(3), C_Y(9), L_X(5), C_Y(7));
    painter.drawLine(L_X(5), C_Y(9), L_X(3), C_Y(7));

    _draw_cross(L_X(1), C_Y(2), painter);
    _draw_cross(L_X(7), C_Y(2), painter);
    _draw_cross(L_X(1), C_Y(7), painter);
    _draw_cross(L_X(7), C_Y(7), painter);
    for (int i = 0; i < 9; i += 2) {
        _draw_cross(L_X(i), C_Y(3), painter);
        _draw_cross(L_X(i), C_Y(6), painter);
    }

    pen.setColor(Qt::darkRed);
    painter.setFont(QFont("微软简隶书", 35, 20));
    painter.setPen(pen);
    painter.drawText(L_X(1), C_Y(5) - 10, QString("楚 河  汉 界"));
}

void Board::init_board_by_manual(ChessManual *manual)
{
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++) {
            if (manual->chesses[i][j] < 0)
                continue;
            chesses.append(Chess(manual->chesses[i][j] % 10, manual->chesses[i][j] / 10, j, i));
        }
}


// 鼠标事件响应
void Board::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        int x = event->pos().x();
        int y = event->pos().y();
        QPoint point = get_chess_pos_by_mouse_pos(x, y);
        Chess *chess = get_chess_by_pos(point.x(), point.y());

        // 选中(取消选中)当前棋子
        if (chess != nullptr && chess->color == current_chess_color) {
            if (choice == chess)
                choice->is_choice = 0, choice = nullptr;
            else {
                if (choice)
                    choice->is_choice = 0;
                chess->is_choice = 1, choice = chess;
            }
        // 移动棋子
        } else if (choice && move_chess_to_pos(choice->x, choice->y, point.x(), point.y())) {
            remove_chess(chess);
            if (is_game_over())
                game_over();
            current_chess_color = (current_chess_color == GREEN ? RED : GREEN);
        }
        update();
    // 鼠标右键取消选中
    } else if (event->button() == Qt::RightButton) {
        if (choice != nullptr)
            choice->is_choice = 0, choice = nullptr;
        update();
    }
}

bool Board::close_event(void)
{
    QString info = tr("你确定要退出？");
    return QMessageBox::information(nullptr, tr("提示"), info, QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes;
}

void Board::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    draw_board(painter);
    for (int i = 0; i < chesses.length(); i++)
        chesses[i].draw(painter);
}


// 删除棋子
void Board::remove_chess(Chess *chess)
{
    for (int i = 0; i < chesses.length(); i++) 
        if (chess == &chesses[i]) {
            chesses.removeAt(i);
            break;
        }
}


static void _draw_cross(int x, int y, QPainter &painter)
{
    if (x != L_X(0)) {
        painter.drawLine(x - 5, y - 5, x - 18, y - 5);
        painter.drawLine(x - 5, y - 5, x - 5, y - 18);
        painter.drawLine(x - 5, y + 5, x - 18, y + 5);
        painter.drawLine(x - 5, y + 5, x - 5, y + 18);
    }
    if (x != L_X(8)) {
        painter.drawLine(x + 5, y - 5, x + 18, y - 5);
        painter.drawLine(x + 5, y - 5, x + 5, y - 18);
        painter.drawLine(x + 5, y + 5, x + 18, y + 5);
        painter.drawLine(x + 5, y + 5, x + 5, y + 18);
    }
}

// 移动棋子到相应位置
void Board::chess_moving()
{
    int x1 = L_X(choice->x), y1 = L_X(choice->y);
    int x2 = L_X(pt_mov_to.x()), y2 = L_X(pt_mov_to.y());

    int dx = x2 - x1;
    int dy = y2 - y1;
    int ux = 20 * (((dx > 0) << 1) - 1); // x的增量方向，取或-1
    int uy = 20 * (((dy > 0) << 1) - 1); // y的增量方向，取或-1
    int eps; // eps为累加误差

    choice->is_moving = 1;
    choice->x = x1, choice->y = y1;
    eps = 0, dx = abs(dx), dy = abs(dy);
    if (dx > dy)
        for (choice->x = x1; choice->x != x2; choice->x += ux)
        {
            repaint();
            eps += dy;
            if ((eps << 1) >= dx)
                choice->y += uy, eps -= dx;
        }
    else
        for (choice->y = y1; choice->y != y2; choice->y += uy)
        {
            repaint(); 
            eps += dx;
            if ((eps << 1) >= dy)
                choice->x += ux, eps -= dy;
        }

    choice->is_moving = 0;
    choice->x = pt_mov_to.x();
    choice->y = pt_mov_to.y();
}

int Board::is_game_over()  
{
    int cnt = 0;
    for (Chess &chess : chesses) 
        if (chess.name == SHUAI && ++cnt == 2) 
                return 0;
    return 1;
}

void Board::game_over()
{
    QString info;
    if (current_chess_color == RED)
        info = QString("游戏结束，红方胜利，您想？");
    else
        info = QString("游戏结束，蓝方胜利");
    QMessageBox message(QMessageBox::Warning, "信息", info);
    message.setStandardButtons((QMessageBox::Yes | QMessageBox::No));
    message.setButtonText(QMessageBox::Yes, "再玩一局");
    message.setButtonText(QMessageBox::No, "退出");
    if (message.exec() == QMessageBox::Yes)
        restart();
    else
        exit(0);
}

void Board::restart()
{
    grid_size = GRID_SIZE;
    ox = OX;
    oy = OY;
    choice = nullptr;
    current_chess_color = 0;
    chesses.clear();
    init_board_by_manual(new ChessManual());
}

