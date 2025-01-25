#ifndef CHESSTYPES_H
#define CHESSTYPES_H

#include <vector>

// --------------------------------------------------
// 1. Basic Data Structures & Definitions
// --------------------------------------------------

// Piece types
enum PieceType {
    EMPTY = 0,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

// Piece colors
enum Color {
    WHITE = 0,
    BLACK = 1,
    NO_COLOR
};

// Simple container for a piece
struct Piece {
    PieceType type;
    Color color;

    Piece(PieceType t = EMPTY, Color c = NO_COLOR)
        : type(t), color(c) {
    }
};

// A move structure: from-square, to-square, etc.
struct Move {
    int fromRow, fromCol;
    int toRow, toCol;
    int score; // Used for sorting or alpha-beta internal scoring

    Move(int fr = 0, int fc = 0, int tr = 0, int tc = 0)
        : fromRow(fr), fromCol(fc), toRow(tr), toCol(tc), score(0) {
    }
};

#endif // CHESSTYPES_H
