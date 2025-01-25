#ifndef EVALUATION_H
#define EVALUATION_H

#include "Board.h" // we need Board, Piece, etc.
#include <random>

// Simple piece-value structure
struct EvalParameters {
    int pawnValue;
    int knightValue;
    int bishopValue;
    int rookValue;
    int queenValue;
};

// For evolving (training) – a struct that holds parameters + fitness
struct Candidate {
    EvalParameters params;
    double fitness;
};

// Evaluate a board with the given parameters (material only, naive)
int evaluateBoard(const Board& b, const EvalParameters& evalParams);

// Mutate parameters randomly
EvalParameters mutateParameters(const EvalParameters& params);

// Functions for “training” (very simplified)
std::vector<Board> createRandomTestPositions(int numPositions);
double measureFitness(const Candidate& cand, const std::vector<Board>& testPositions);
EvalParameters trainEvalParameters(int generations, int populationSize, int testPositionsCount);

#endif // EVALUATION_H
