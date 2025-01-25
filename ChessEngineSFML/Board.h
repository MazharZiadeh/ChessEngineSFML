#ifndef BOARD_H
#define BOARD_H

#include "ChessTypes.h"

class Board {
public:
    static const int SIZE = 8;
    Piece board[SIZE][SIZE];
    Color sideToMove; // 0 = WHITE, 1 = BLACK

    Board();
    void initBoard();
    bool inBounds(int r, int c) const;

    // Generate pseudo-legal moves (simplified)
    std::vector<Move> generateMoves();

    // Execute / Undo moves
    void makeMove(const Move& m);
    void undoMove(const Move& m, Piece captured);

private:
    // Internal helpers for move generation
    void generatePawnMoves(int r, int c, std::vector<Move>& moves);
    void generateKnightMoves(int r, int c, std::vector<Move>& moves);
    void generateSlidingMoves(int r, int c, std::vector<Move>& moves,
        const std::vector<std::pair<int, int>>& directions);
    void generateKingMoves(int r, int c, std::vector<Move>& moves);
};

#endif // BOARD_H
