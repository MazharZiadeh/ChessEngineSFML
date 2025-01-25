#ifndef MINIMAX_H
#define MINIMAX_H

#include "Evaluation.h" // we need evaluateBoard, EvalParameters
#include "Board.h"

// Alpha-Beta search
int alphaBeta(Board& b, int depth, int alpha, int beta, const EvalParameters& evalParams);

// Returns the best move
Move findBestMove(Board& b, int depth, const EvalParameters& evalParams);

#endif // MINIMAX_H
