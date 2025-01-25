#include <SFML/Graphics.hpp>
#include <iostream>

#include "Board.h"
#include "Evaluation.h"
#include "Minimax.h"

// --------------------------------------------------
// SFML GUI Helpers
// --------------------------------------------------
static const float TILE_SIZE = 80.f; // pixels

sf::Vector2f getTilePosition(int row, int col) {
    // We draw row 0 at the bottom visually => invert for top
    return sf::Vector2f(col * TILE_SIZE, (7 - row) * TILE_SIZE);
}

sf::Color getPieceColor(const Piece& p) {
    if (p.color == WHITE)  return sf::Color::White;
    if (p.color == BLACK)  return sf::Color::Black;
    return sf::Color::Transparent;
}

// Create a circle shape to represent the piece
sf::CircleShape createPieceShape(const Piece& p) {
    sf::CircleShape shape(TILE_SIZE / 2.5f);
    shape.setOrigin(TILE_SIZE / 2.5f, TILE_SIZE / 2.5f);

    sf::Color color = getPieceColor(p);
    shape.setFillColor(color);

    // Outline indicates piece type
    if (p.type == KING)   shape.setOutlineColor(sf::Color::Red);
    if (p.type == QUEEN)  shape.setOutlineColor(sf::Color::Green);
    if (p.type == ROOK)   shape.setOutlineColor(sf::Color::Blue);
    if (p.type == BISHOP) shape.setOutlineColor(sf::Color::Magenta);
    if (p.type == KNIGHT) shape.setOutlineColor(sf::Color::Cyan);
    if (p.type == PAWN)   shape.setOutlineColor(sf::Color::Yellow);

    shape.setOutlineThickness(3.f);

    return shape;
}

// --------------------------------------------------
// Main
// --------------------------------------------------
int main() {
    // Optionally: run a brief "training" to find better eval parameters.
    // If you want to skip it (since it can be slow), just comment it out.
    std::cout << "Starting optional evolutionary parameter training...\n";
    EvalParameters bestParams = trainEvalParameters(
        /*generations=*/2,
        /*populationSize=*/6,
        /*testPositionsCount=*/2
    );
    std::cout << "Training done. Best parameters found:\n"
        << "  Pawn: " << bestParams.pawnValue << "\n"
        << "  Knight: " << bestParams.knightValue << "\n"
        << "  Bishop: " << bestParams.bishopValue << "\n"
        << "  Rook: " << bestParams.rookValue << "\n"
        << "  Queen: " << bestParams.queenValue << "\n";

    // Create an SFML window
    sf::RenderWindow window(sf::VideoMode(640, 640), "Chess Engine (GUI)");
    window.setFramerateLimit(60);

    // Initialize board
    Board board;

    bool isDragging = false;
    sf::Vector2i dragFrom(-1, -1);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle user mouse input for making a move (human = WHITE)
            if (board.sideToMove == WHITE) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        int col = mousePos.x / (int)TILE_SIZE;
                        int row = 7 - (mousePos.y / (int)TILE_SIZE);

                        if (!isDragging) {
                            dragFrom = sf::Vector2i(row, col);
                            isDragging = true;
                        }
                    }
                }
                if (event.type == sf::Event::MouseButtonReleased) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        int col = mousePos.x / (int)TILE_SIZE;
                        int row = 7 - (mousePos.y / (int)TILE_SIZE);

                        if (isDragging) {
                            // Attempt user move
                            Move userMove(dragFrom.x, dragFrom.y, row, col);
                            std::vector<Move> legalMoves = board.generateMoves();
                            bool found = false;
                            for (auto& m : legalMoves) {
                                if (m.fromRow == userMove.fromRow &&
                                    m.fromCol == userMove.fromCol &&
                                    m.toRow == userMove.toRow &&
                                    m.toCol == userMove.toCol) {
                                    board.makeMove(m);
                                    found = true;
                                    break;
                                }
                            }
                            // If not found, invalid move => do nothing
                        }
                        isDragging = false;
                    }
                }
            }
        }

        // If it's AI's turn (BLACK), let the AI move
        if (board.sideToMove == BLACK) {
            int searchDepth = 4;
            Move best = findBestMove(board, searchDepth, bestParams);
            if (!(best.fromRow == best.toRow && best.fromCol == best.toCol)) {
                board.makeMove(best);
            }
        }

        // Draw the board
        window.clear(sf::Color::White);

        // Draw chessboard squares
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                sf::RectangleShape rect(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                rect.setPosition(getTilePosition(r, c));
                if ((r + c) % 2 == 0) {
                    rect.setFillColor(sf::Color(209, 139, 71)); // dark square
                }
                else {
                    rect.setFillColor(sf::Color(255, 206, 158)); // light square
                }
                window.draw(rect);
            }
        }

        // Draw pieces
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                Piece p = board.board[r][c];
                if (p.type != EMPTY) {
                    sf::CircleShape shape = createPieceShape(p);
                    sf::Vector2f tilePos = getTilePosition(r, c);
                    shape.setPosition(tilePos.x + TILE_SIZE / 2.f,
                        tilePos.y + TILE_SIZE / 2.f);
                    window.draw(shape);
                }
            }
        }

        window.display();
    }

    return 0;
}
