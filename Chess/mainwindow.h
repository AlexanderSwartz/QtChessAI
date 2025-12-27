#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QLabel>
#include "ChessBoard.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QVector<QVector<QLabel*>> boardLabels; // 8x8 grid
    ChessBoard board;
    int selectedRow, selectedCol;
    QVector<QVector<bool>> hasMoveIndicator;

    void setupChessBoard();
    void handleSquareClick(int visualRow, int col);
    void clearSelection();
    void clearMoveIndicators();
    void showPawnMoves(int boardRow, int col, Piece p);
    void showKnightMoves(int boardRow, int col, Piece p);
    void updateBoardDisplay();
    bool isWhitePiece(Piece p);

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // MAINWINDOW_H
