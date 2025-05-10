// Piece.h
#pragma once
#include <SFML/Graphics.hpp>

enum class PieceType { PAWN };
enum class PieceColor { WHITE, BLACK };

// Represents a chess piece
class Piece {
public:
    PieceType type;
    PieceColor color;
    sf::Vector2i position;

    Piece(PieceType t, PieceColor c) : type(t), color(c) {}

    // Returns the texture rectangle for drawing the piece sprite
    sf::IntRect getTextureRect() {
        int row = (color == PieceColor::WHITE) ? 0 : 1;
        int col = 0; // Assuming only pawns for simplicity
        return sf::IntRect(col * 80, row * 80, 80, 80);
    }
};
