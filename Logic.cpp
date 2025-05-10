// Logic.cpp
#include "Logic.h"

// Get pawn moves recursively
void getLegalMovesRecursive(Piece* board[8][8], Piece* piece, int x, int y, std::vector<sf::Vector2i>& moves) {
    if (!piece) return;
    if (piece->type == PieceType::PAWN) {
        int direction = (piece->color == PieceColor::WHITE) ? -1 : 1;
        int nextY = y + direction;

        if (nextY >= 0 && nextY < 8 && board[nextY][x] == nullptr) {
            moves.push_back({x, nextY});
            // Recurse for double step move
            if ((piece->color == PieceColor::WHITE && y == 6) ||
                (piece->color == PieceColor::BLACK && y == 1)) {
                int secondY = y + 2 * direction;
                if (board[secondY][x] == nullptr)
                    getLegalMovesRecursive(board, piece, x, secondY, moves);
            }
        }
    }
}
