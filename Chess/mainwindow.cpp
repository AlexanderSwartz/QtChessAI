#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>

const QString pieceFiles[13] = {"", "w-pawn.png", "w-knight.png", "w-bishop.png", "w-rook.png", "w-queen.png", "w-king.png", "b-pawn.png", "b-knight.png", "b-bishop.png", "b-rook.png", "b-queen.png", "b-king.png"};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , selectedRow(-1)
    , selectedCol(-1)
{
    ui->setupUi(this);
    setupChessBoard();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupChessBoard()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QGridLayout *grid = new QGridLayout(central);
    grid->setSpacing(0);
    grid->setHorizontalSpacing(0);
    grid->setVerticalSpacing(0);
    grid->setContentsMargins(0,0,0,0);
    grid->setAlignment(Qt::AlignCenter);

    central->setContentsMargins(0,0,0,0);

    for (int i = 0; i < 8; ++i)
    {
        grid->setRowStretch(i, 1);
        grid->setColumnStretch(i, 1);
    }


    boardLabels.resize(8);
    hasMoveIndicator.resize(8);
    for (int i = 0; i < 8; ++i)
    {
        hasMoveIndicator[i].resize(8);
        for (int j = 0; j < 8; ++j)
            hasMoveIndicator[i][j] = false;
    }
    for (int visualRow = 0; visualRow < 8; ++visualRow)
    {
        boardLabels[visualRow].resize(8);
        int boardRow = 7 - visualRow; // visualRow 0 is boardRow 7 (black's side)
        for (int col = 0; col < 8; ++col)
        {
            QLabel *square = new QLabel(central);
            square->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            square->setAlignment(Qt::AlignCenter);
            square->setContentsMargins(0, 0, 0, 0);
            square->setFrameStyle(QFrame::NoFrame);

            // Get piece first
            Piece p = board.getPiece(boardRow, col);

            // Build stylesheet (background only, no text color)
            QString bgColor = ((visualRow + col) % 2 == 0) ? "#f0d9b5" : "#b58863";
            QString style = QString("background-color: %1; margin: 0; padding: 0; border: none;").arg(bgColor);
            square->setStyleSheet(style);

            // Set piece image
            if (p != EMPTY) {
                QPixmap pixmap("../Chess/pieces/" + pieceFiles[p]);
                square->setPixmap(pixmap);
                square->setScaledContents(true);
            } else {
                square->setPixmap(QPixmap()); // clear pixmap
            }

            grid->addWidget(square, visualRow, col);
            boardLabels[visualRow][col] = square;
            square->installEventFilter(this);
        }
    }

    central->setLayout(grid);
}

// Override resizeEvent to enforce square window
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    QSize currentSize = size();
    int minDim = qMin(currentSize.width(), currentSize.height());
    if (currentSize.width() != minDim || currentSize.height() != minDim) {
        resize(minDim, minDim);
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QLabel *label = qobject_cast<QLabel*>(obj);
        if (label) {
            for (int r = 0; r < 8; ++r) {
                for (int c = 0; c < 8; ++c) {
                    if (boardLabels[r][c] == label) {
                        handleSquareClick(r, c);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void MainWindow::handleSquareClick(int visualRow, int col)
{
    int boardRow = 7 - visualRow;
    if (selectedRow == -1) {
        Piece p = board.getPiece(boardRow, col);
        if (p != EMPTY) {
            selectedRow = visualRow;
            selectedCol = col;
            boardLabels[visualRow][col]->setStyleSheet("background-color: green; margin: 0; padding: 0; border: none;");
            if (p == WPAWN || p == BPAWN) {
                showPawnMoves(boardRow, col, p);
            } else if (p == WKNIGHT || p == BKNIGHT) {
                showKnightMoves(boardRow, col, p);
            } else if (p == WBISHOP || p == BBISHOP) {
                showBishopMoves(boardRow, col, p);
            } else if (p == WROOK || p == BROOK) {
                showRookMoves(boardRow, col, p);
            } else if (p == WQUEEN || p == BQUEEN) {
                showQueenMoves(boardRow, col, p);
            } else if (p == WKING || p == BKING) {
                showKingMoves(boardRow, col, p);
            }
        }
    } else {
        if (visualRow == selectedRow && col == selectedCol) {
            clearSelection();
        } else {
            Piece target = board.getPiece(boardRow, col);
            Piece selectedPiece = board.getPiece(7 - selectedRow, selectedCol);
            if (target == EMPTY || isWhitePiece(selectedPiece) != isWhitePiece(target)) {
                board.setPiece(7 - selectedRow, selectedCol, EMPTY);
                board.setPiece(boardRow, col, selectedPiece);
                updateBoardDisplay();
                clearSelection();
            } else {
                clearSelection();
            }
        }
    }
}

void MainWindow::clearSelection()
{
    if (selectedRow != -1) {
        QString bgColor = ((selectedRow + selectedCol) % 2 == 0) ? "#f0d9b5" : "#b58863";
        boardLabels[selectedRow][selectedCol]->setStyleSheet(QString("background-color: %1; margin: 0; padding: 0; border: none;").arg(bgColor));
        selectedRow = -1;
        selectedCol = -1;
    }
    clearMoveIndicators();
}

void MainWindow::clearMoveIndicators()
{
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (hasMoveIndicator[r][c]) {
                int boardR = 7 - r;
                Piece p = board.getPiece(boardR, c);
                if (p != EMPTY) {
                    boardLabels[r][c]->setPixmap(QPixmap("../Chess/pieces/" + pieceFiles[p]));
                } else {
                    boardLabels[r][c]->setPixmap(QPixmap());
                }
                hasMoveIndicator[r][c] = false;
            }
        }
    }
}

void MainWindow::showPawnMoves(int boardRow, int col, Piece p)
{
    int dir = (p == WPAWN) ? 1 : -1;
    int newRow = boardRow + dir;
    if (newRow >= 0 && newRow < 8 && board.getPiece(newRow, col) == EMPTY) {
        int visualR = 7 - newRow;
        boardLabels[visualR][col]->setPixmap(QPixmap("../Chess/pieces/g-dot2.png"));
        boardLabels[visualR][col]->setScaledContents(true);
        hasMoveIndicator[visualR][col] = true;
    }
    if ((p == WPAWN && boardRow == 1) || (p == BPAWN && boardRow == 6)) {
        int doubleRow = boardRow + 2 * dir;
        if (board.getPiece(boardRow + dir, col) == EMPTY && board.getPiece(doubleRow, col) == EMPTY) {
            int visualR = 7 - doubleRow;
            boardLabels[visualR][col]->setPixmap(QPixmap("../Chess/pieces/g-dot2.png"));
            boardLabels[visualR][col]->setScaledContents(true);
            hasMoveIndicator[visualR][col] = true;
        }
    }
    for (int dc = -1; dc <= 1; dc += 2) {
        int newC = col + dc;
        if (newC >= 0 && newC < 8) {
            int capRow = boardRow + dir;
            Piece target = board.getPiece(capRow, newC);
            if (target != EMPTY && isWhitePiece(target) != isWhitePiece(p)) {
                int visualR = 7 - capRow;
                boardLabels[visualR][newC]->setPixmap(QPixmap("../Chess/pieces/g-dot2.png"));
                boardLabels[visualR][newC]->setScaledContents(true);
                hasMoveIndicator[visualR][newC] = true;
            }
        }
    }
}

void MainWindow::showKnightMoves(int boardRow, int col, Piece p)
{
    // Knight moves: 2 in one direction, 1 perpendicular
    int deltas[8][2] = {
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2},
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}
    };

    for (int i = 0; i < 8; ++i) {
        int newRow = boardRow + deltas[i][0];
        int newCol = col + deltas[i][1];
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            Piece target = board.getPiece(newRow, newCol);
            if (target == EMPTY || isWhitePiece(target) != isWhitePiece(p)) {
                int visualR = 7 - newRow;
                boardLabels[visualR][newCol]->setPixmap(QPixmap("../Chess/pieces/g-dot2.png"));
                boardLabels[visualR][newCol]->setScaledContents(true);
                hasMoveIndicator[visualR][newCol] = true;
            }
        }
    }
}

void MainWindow::showBishopMoves(int boardRow, int col, Piece p)
{
    int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (int d = 0; d < 4; ++d) {
        int dr = directions[d][0];
        int dc = directions[d][1];
        int r = boardRow + dr;
        int c = col + dc;
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            Piece target = board.getPiece(r, c);
            if (target == EMPTY) {
                int visualR = 7 - r;
                boardLabels[visualR][c]->setPixmap(QPixmap("../Chess/pieces/g-dot2.png"));
                boardLabels[visualR][c]->setScaledContents(true);
                hasMoveIndicator[visualR][c] = true;
            } else {
                if (isWhitePiece(target) != isWhitePiece(p)) {
                    int visualR = 7 - r;
                    boardLabels[visualR][c]->setPixmap(QPixmap("../Chess/pieces/g-dot2.png"));
                    boardLabels[visualR][c]->setScaledContents(true);
                    hasMoveIndicator[visualR][c] = true;
                }
                break;
            }
            r += dr;
            c += dc;
        }
    }
}

void MainWindow::showRookMoves(int boardRow, int col, Piece p)
{
    int directions[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    for (int d = 0; d < 4; ++d) {
        int dr = directions[d][0];
        int dc = directions[d][1];
        int r = boardRow + dr;
        int c = col + dc;
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            Piece target = board.getPiece(r, c);
            if (target == EMPTY) {
                int visualR = 7 - r;
                boardLabels[visualR][c]->setPixmap(QPixmap("../Chess/pieces/g-dot2.png"));
                boardLabels[visualR][c]->setScaledContents(true);
                hasMoveIndicator[visualR][c] = true;
            } else {
                if (isWhitePiece(target) != isWhitePiece(p)) {
                    int visualR = 7 - r;
                    boardLabels[visualR][c]->setPixmap(QPixmap("../Chess/pieces/g-dot2.png"));
                    boardLabels[visualR][c]->setScaledContents(true);
                    hasMoveIndicator[visualR][c] = true;
                }
                break;
            }
            r += dr;
            c += dc;
        }
    }
}

void MainWindow::showQueenMoves(int boardRow, int col, Piece p)
{
    int directions[8][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (int d = 0; d < 8; ++d) {
        int dr = directions[d][0];
        int dc = directions[d][1];
        int r = boardRow + dr;
        int c = col + dc;
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            Piece target = board.getPiece(r, c);
            if (target == EMPTY) {
                int visualR = 7 - r;
                boardLabels[visualR][c]->setPixmap(QPixmap("../Chess/pieces/g-dot2.png"));
                boardLabels[visualR][c]->setScaledContents(true);
                hasMoveIndicator[visualR][c] = true;
            } else {
                if (isWhitePiece(target) != isWhitePiece(p)) {
                    int visualR = 7 - r;
                    boardLabels[visualR][c]->setPixmap(QPixmap("../Chess/pieces/g-dot2.png"));
                    boardLabels[visualR][c]->setScaledContents(true);
                    hasMoveIndicator[visualR][c] = true;
                }
                break;
            }
            r += dr;
            c += dc;
        }
    }
}

void MainWindow::showKingMoves(int boardRow, int col, Piece p)
{
    int directions[8][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (int d = 0; d < 8; ++d) {
        int r = boardRow + directions[d][0];
        int c = col + directions[d][1];
        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
            Piece target = board.getPiece(r, c);
            if (target == EMPTY || isWhitePiece(target) != isWhitePiece(p)) {
                int visualR = 7 - r;
                boardLabels[visualR][c]->setPixmap(QPixmap("../Chess/pieces/g-dot2.png"));
                boardLabels[visualR][c]->setScaledContents(true);
                hasMoveIndicator[visualR][c] = true;
            }
        }
    }
}

void MainWindow::updateBoardDisplay()
{
    for (int vr = 0; vr < 8; ++vr) {
        for (int c = 0; c < 8; ++c) {
            int br = 7 - vr;
            Piece p = board.getPiece(br, c);
            QString bgColor = ((vr + c) % 2 == 0) ? "#f0d9b5" : "#b58863";
            boardLabels[vr][c]->setStyleSheet(QString("background-color: %1; margin: 0; padding: 0; border: none;").arg(bgColor));
            if (p != EMPTY) {
                boardLabels[vr][c]->setPixmap(QPixmap("../Chess/pieces/" + pieceFiles[p]));
                boardLabels[vr][c]->setScaledContents(true);
            } else {
                boardLabels[vr][c]->setPixmap(QPixmap());
            }
        }
    }
}

bool MainWindow::isWhitePiece(Piece p)
{
    return p >= WPAWN && p <= WKING;
}

