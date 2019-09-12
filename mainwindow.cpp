#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Board.h"

#include <qevent.h>
#include <QHBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QHBoxLayout *box_layout = new QHBoxLayout;
    board = new Board;
    board->setFixedWidth(ui->graphicsView->width());
    board->setFixedHeight(ui->graphicsView->height());
    box_layout->addWidget(board);
    ui->graphicsView->setLayout(box_layout);
    connect(ui->action, SIGNAL(triggered()), this, SLOT(show_about()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete board;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (board->close_event())
        event->accept();
    else
        event->ignore();
}

void MainWindow::show_about()
{
    QMessageBox::information(nullptr, "关于", "中国象棋 V1.0", QMessageBox::Yes);
}
