// Logic.h
#pragma once
#include <vector>
#include "Piece.h"

// Recursive function to generate pawn moves
void getLegalMovesRecursive(Piece* board[8][8], Piece* piece, int x, int y, std::vector<sf::Vector2i>& moves);
