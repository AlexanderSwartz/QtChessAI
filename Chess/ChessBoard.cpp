#include "ChessBoard.h"
#include <iostream>

ChessBoard::ChessBoard() {
    initialize();
}

void ChessBoard::initialize() {
    // Clear all bitboards
    for (auto& bb : bitboards) {
        bb = 0ULL;
    }

    // Set initial positions
    // White pawns on row 1 (0-indexed as 1)
    for (int col = 0; col < 8; ++col) {
        setPiece(1, col, WPAWN);
    }
    // White pieces on row 0
    setPiece(0, 0, WROOK);
    setPiece(0, 1, WKNIGHT);
    setPiece(0, 2, WBISHOP);
    setPiece(0, 3, WQUEEN);
    setPiece(0, 4, WKING);
    setPiece(0, 5, WBISHOP);
    setPiece(0, 6, WKNIGHT);
    setPiece(0, 7, WROOK);

    // Black pawns on row 6
    for (int col = 0; col < 8; ++col) {
        setPiece(6, col, BPAWN);
    }
    // Black pieces on row 7
    setPiece(7, 0, BROOK);
    setPiece(7, 1, BKNIGHT);
    setPiece(7, 2, BBISHOP);
    setPiece(7, 3, BQUEEN);
    setPiece(7, 4, BKING);
    setPiece(7, 5, BBISHOP);
    setPiece(7, 6, BKNIGHT);
    setPiece(7, 7, BROOK);
}

void ChessBoard::setPiece(int row, int col, Piece piece) {
    // Clear the bit at this position for all pieces
    uint64_t bit = getBit(row, col);
    for (auto& bb : bitboards) {
        bb &= ~bit;
    }
    // Set the bit for the new piece if not empty
    if (piece != EMPTY) {
        bitboards[piece - 1] |= bit;
    }
}

Piece ChessBoard::getPiece(int row, int col) const {
    uint64_t bit = getBit(row, col);
    for (int i = 0; i < 12; ++i) {
        if (bitboards[i] & bit) {
            return static_cast<Piece>(i + 1);
        }
    }
    return EMPTY;
}

uint64_t ChessBoard::getBit(int row, int col) const {
    return 1ULL << (row * 8 + col);
}

std::string ChessBoard::toString() const {
    std::string boardStr;
    char pieceChars[13] = {'.', 'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};
    for (int row = 7; row >= 0; --row) { // Start from top (black side)
        for (int col = 0; col < 8; ++col) {
            Piece p = getPiece(row, col);
            boardStr += pieceChars[p];
            boardStr += ' ';
        }
        boardStr += '\n';
    }
    return boardStr;
}