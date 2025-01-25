#include "Evaluation.h"
#include <cmath>      // for std::abs
#include <algorithm>  // for std::sort
#include <iostream>   // for std::cout, debugging
#include <ctime>

int evaluateBoard(const Board& b, const EvalParameters& evalParams) {
    int score = 0;
    for (int r = 0; r < Board::SIZE; r++) {
        for (int c = 0; c < Board::SIZE; c++) {
            const Piece& p = b.board[r][c];
            int val = 0;
            switch (p.type) {
            case PAWN:   val = evalParams.pawnValue;   break;
            case KNIGHT: val = evalParams.knightValue; break;
            case BISHOP: val = evalParams.bishopValue; break;
            case ROOK:   val = evalParams.rookValue;   break;
            case QUEEN:  val = evalParams.queenValue;  break;
            default:     val = 0;                      break;
            }
            if (p.color == BLACK) {
                val = -val;
            }
            score += val;
        }
    }
    return score;
}

EvalParameters mutateParameters(const EvalParameters& params) {
    static std::mt19937 rng(static_cast<unsigned>(time(nullptr)));
    std::uniform_int_distribution<int> dist(-10, 10);

    EvalParameters newParams = params;
    newParams.pawnValue += dist(rng);
    newParams.knightValue += dist(rng);
    newParams.bishopValue += dist(rng);
    newParams.rookValue += dist(rng);
    newParams.queenValue += dist(rng);

    return newParams;
}

// Create random boards for testing (very simplistic)
std::vector<Board> createRandomTestPositions(int numPositions) {
    std::vector<Board> positions;
    positions.reserve(numPositions);

    for (int i = 0; i < numPositions; i++) {
        // For now, just push standard boards, or do minimal randomization
        Board b;
        positions.push_back(b);
    }
    return positions;
}

// Evaluate candidate on test positions
double measureFitness(const Candidate& cand, const std::vector<Board>& testPositions) {
    double sumScores = 0.0;
    for (const auto& b : testPositions) {
        int score = evaluateBoard(b, cand.params);
        // We'll do a silly metric: prefer eval near 0 
        // to avoid big +/- swings: fitness = -abs(score)/100
        sumScores -= std::abs(score) / 100.0;
    }
    return sumScores;
}

EvalParameters trainEvalParameters(int generations, int populationSize, int testPositionsCount) {
    // 1) Create initial population
    std::mt19937 rng(static_cast<unsigned>(time(nullptr)));
    std::uniform_int_distribution<int> distBase(80, 120); // near typical values

    std::vector<Candidate> population(populationSize);
    for (int i = 0; i < populationSize; i++) {
        population[i].params.pawnValue = distBase(rng);
        population[i].params.knightValue = distBase(rng) * 3;
        population[i].params.bishopValue = distBase(rng) * 3;
        population[i].params.rookValue = distBase(rng) * 5;
        population[i].params.queenValue = distBase(rng) * 9;
        population[i].fitness = 0.0;
    }

    // 2) Make test positions
    auto testPositions = createRandomTestPositions(testPositionsCount);

    // 3) Evolution loop
    for (int gen = 0; gen < generations; gen++) {
        // Evaluate each candidate
        for (auto& cand : population) {
            cand.fitness = measureFitness(cand, testPositions);
        }

        // Sort by fitness descending
        std::sort(population.begin(), population.end(),
            [](const Candidate& a, const Candidate& b) {
                return a.fitness > b.fitness;
            });

        // Print best for debug
        std::cout << "Gen " << gen
            << " best fitness: " << population[0].fitness << std::endl;

        // Keep top half, mutate to fill the bottom half
        int survivors = populationSize / 2;
        for (int i = survivors; i < populationSize; i++) {
            population[i].params = mutateParameters(
                population[i - survivors].params
            );
        }
    }

    return population[0].params;
}
