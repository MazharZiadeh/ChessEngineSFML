#include "Board.h"

#include <algorithm> // for std::max, std::min

Board::Board() {
    initBoard();
    sideToMove = WHITE;
}

// Initialize standard chess board
void Board::initBoard() {
    // Clear
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            board[r][c] = Piece(EMPTY, NO_COLOR);
        }
    }

    // Pawns
    for (int c = 0; c < SIZE; c++) {
        board[1][c] = Piece(PAWN, WHITE);
        board[6][c] = Piece(PAWN, BLACK);
    }

    // Rooks
    board[0][0] = Piece(ROOK, WHITE);
    board[0][7] = Piece(ROOK, WHITE);
    board[7][0] = Piece(ROOK, BLACK);
    board[7][7] = Piece(ROOK, BLACK);

    // Knights
    board[0][1] = Piece(KNIGHT, WHITE);
    board[0][6] = Piece(KNIGHT, WHITE);
    board[7][1] = Piece(KNIGHT, BLACK);
    board[7][6] = Piece(KNIGHT, BLACK);

    // Bishops
    board[0][2] = Piece(BISHOP, WHITE);
    board[0][5] = Piece(BISHOP, WHITE);
    board[7][2] = Piece(BISHOP, BLACK);
    board[7][5] = Piece(BISHOP, BLACK);

    // Queens
    board[0][3] = Piece(QUEEN, WHITE);
    board[7][3] = Piece(QUEEN, BLACK);

    // Kings
    board[0][4] = Piece(KING, WHITE);
    board[7][4] = Piece(KING, BLACK);
}

bool Board::inBounds(int r, int c) const {
    return (r >= 0 && r < SIZE && c >= 0 && c < SIZE);
}

std::vector<Move> Board::generateMoves() {
    std::vector<Move> moves;
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            Piece piece = board[r][c];
            if (piece.color == sideToMove) {
                switch (piece.type) {
                case PAWN:
                    generatePawnMoves(r, c, moves);
                    break;
                case KNIGHT:
                    generateKnightMoves(r, c, moves);
                    break;
                case BISHOP:
                    generateSlidingMoves(r, c, moves, { {1,1},{1,-1},{-1,1},{-1,-1} });
                    break;
                case ROOK:
                    generateSlidingMoves(r, c, moves, { {1,0},{-1,0},{0,1},{0,-1} });
                    break;
                case QUEEN:
                    generateSlidingMoves(r, c, moves, {
                        {1,0},{-1,0},{0,1},{0,-1},
                        {1,1},{1,-1},{-1,1},{-1,-1}
                        });
                    break;
                case KING:
                    generateKingMoves(r, c, moves);
                    break;
                default:
                    break;
                }
            }
        }
    }
    return moves;
}

// Make a move on the board
void Board::makeMove(const Move& m) {
    Piece& src = board[m.fromRow][m.fromCol];
    Piece& dst = board[m.toRow][m.toCol];

    // Move piece
    dst = src;
    src = Piece(EMPTY, NO_COLOR);

    // Pawn promotion (simplified: auto-queen)
    if (dst.type == PAWN) {
        if ((dst.color == WHITE && m.toRow == 7) ||
            (dst.color == BLACK && m.toRow == 0)) {
            dst.type = QUEEN;
        }
    }

    // Switch side
    sideToMove = (sideToMove == WHITE ? BLACK : WHITE);
}

// Undo move
void Board::undoMove(const Move& m, Piece captured) {
    Piece& src = board[m.fromRow][m.fromCol];
    Piece& dst = board[m.toRow][m.toCol];

    // Restore
    src = dst;
    dst = captured;

    // Pawn promotion revert
    if (src.type == QUEEN &&
        ((m.toRow == 7 && src.color == WHITE) ||
            (m.toRow == 0 && src.color == BLACK))) {
        src.type = PAWN;
    }

    // Switch side back
    sideToMove = (sideToMove == WHITE ? BLACK : WHITE);
}

// --------------------------
// Private Move Generators
// --------------------------
void Board::generatePawnMoves(int r, int c, std::vector<Move>& moves) {
    Piece piece = board[r][c];
    int dir = (piece.color == WHITE) ? 1 : -1;
    int fr = r + dir;

    // Move forward if empty
    if (inBounds(fr, c) && board[fr][c].type == EMPTY) {
        moves.push_back(Move(r, c, fr, c));
    }
    // Capture diagonals
    for (int dc = -1; dc <= 1; dc += 2) {
        int fc = c + dc;
        if (inBounds(fr, fc) && board[fr][fc].type != EMPTY
            && board[fr][fc].color != piece.color) {
            moves.push_back(Move(r, c, fr, fc));
        }
    }
}

void Board::generateKnightMoves(int r, int c, std::vector<Move>& moves) {
    static const int offsets[8][2] = {
        {2,1},{2,-1},{-2,1},{-2,-1},
        {1,2},{1,-2},{-1,2},{-1,-2}
    };
    Piece piece = board[r][c];
    for (auto& off : offsets) {
        int nr = r + off[0];
        int nc = c + off[1];
        if (inBounds(nr, nc)) {
            if (board[nr][nc].color != piece.color) {
                moves.push_back(Move(r, c, nr, nc));
            }
        }
    }
}

void Board::generateSlidingMoves(int r, int c, std::vector<Move>& moves,
    const std::vector<std::pair<int, int>>& directions)
{
    Piece piece = board[r][c];
    for (auto& d : directions) {
        int nr = r, nc = c;
        while (true) {
            nr += d.first;
            nc += d.second;
            if (!inBounds(nr, nc)) break;
            if (board[nr][nc].type == EMPTY) {
                moves.push_back(Move(r, c, nr, nc));
            }
            else {
                // Capture?
                if (board[nr][nc].color != piece.color) {
                    moves.push_back(Move(r, c, nr, nc));
                }
                break; // blocked
            }
        }
    }
}

void Board::generateKingMoves(int r, int c, std::vector<Move>& moves) {
    Piece piece = board[r][c];
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int nr = r + dr;
            int nc = c + dc;
            if (inBounds(nr, nc)) {
                if (board[nr][nc].color != piece.color) {
                    moves.push_back(Move(r, c, nr, nc));
                }
            }
        }
    }
}

