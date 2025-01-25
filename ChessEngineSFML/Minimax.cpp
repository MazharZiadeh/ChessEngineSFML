#include "Minimax.h"
#include <limits>

int alphaBeta(Board& b, int depth, int alpha, int beta, const EvalParameters& evalParams) {
    if (depth == 0) {
        return evaluateBoard(b, evalParams);
    }

    std::vector<Move> moves = b.generateMoves();
    if (moves.empty()) {
        // No moves -> checkmate/stalemate
        // Just return static eval in this simplistic approach
        return evaluateBoard(b, evalParams);
    }

    if (b.sideToMove == WHITE) {
        int value = -999999;
        for (auto& m : moves) {
            Piece captured = b.board[m.toRow][m.toCol];
            b.makeMove(m);

            value = std::max(value, alphaBeta(b, depth - 1, alpha, beta, evalParams));

            b.undoMove(m, captured);

            alpha = std::max(alpha, value);
            if (alpha >= beta) break; // beta cutoff
        }
        return value;
    }
    else {
        int value = 999999;
        for (auto& m : moves) {
            Piece captured = b.board[m.toRow][m.toCol];
            b.makeMove(m);

            value = std::min(value, alphaBeta(b, depth - 1, alpha, beta, evalParams));

            b.undoMove(m, captured);

            beta = std::min(beta, value);
            if (beta <= alpha) break; // alpha cutoff
        }
        return value;
    }
}

Move findBestMove(Board& b, int depth, const EvalParameters& evalParams) {
    std::vector<Move> moves = b.generateMoves();
    Move bestMove;

    if (moves.empty()) {
        // No moves
        return bestMove;
    }

    int bestScore = (b.sideToMove == WHITE) ? -999999 : 999999;

    for (auto& m : moves) {
        Piece captured = b.board[m.toRow][m.toCol];
        b.makeMove(m);

        int score = alphaBeta(b, depth - 1, -1000000, 1000000, evalParams);

        b.undoMove(m, captured);

        if (b.sideToMove == WHITE) {
            if (score > bestScore) {
                bestScore = score;
                bestMove = m;
            }
        }
        else {
            if (score < bestScore) {
                bestScore = score;
                bestMove = m;
            }
        }
    }
    return bestMove;
}
