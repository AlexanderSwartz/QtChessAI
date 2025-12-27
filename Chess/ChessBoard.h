#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <cstdint>
#include <array>
#include <string>

enum Piece {
    EMPTY = 0,
    WPAWN, WKNIGHT, WBISHOP, WROOK, WQUEEN, WKING,
    BPAWN, BKNIGHT, BBISHOP, BROOK, BQUEEN, BKING
};

class ChessBoard {
public:
    ChessBoard();
    void initialize();
    void setPiece(int row, int col, Piece piece);
    Piece getPiece(int row, int col) const;
    std::string toString() const;

private:
    std::array<uint64_t, 12> bitboards; // One for each piece type
    uint64_t getBit(int row, int col) const;
    void setBit(int row, int col, bool value, Piece piece);
};

#endif // CHESSBOARD_H