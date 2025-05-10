// ChessGame.cpp
// A simplified chess game using SFML with recursive move generation and basic interactivity

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Piece.h"
#include "Logic.h"

const int TILE_SIZE = 80;
const int BOARD_SIZE = 8;

// Chessboard with nullptrs or Piece pointers
Piece* board[BOARD_SIZE][BOARD_SIZE] = {nullptr};
PieceColor currentTurn = PieceColor::WHITE;

// Currently selected piece and its possible moves
Piece* selectedPiece = nullptr;
std::vector<sf::Vector2i> possibleMoves;

// Draw the board and pieces
void drawBoard(sf::RenderWindow& window, sf::Texture& texture) {
    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));

    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            bool isWhite = (x + y) % 2 == 0;
            tile.setFillColor(isWhite ? sf::Color::White : sf::Color(128, 128, 128));
            tile.setPosition(x * TILE_SIZE, y * TILE_SIZE);
            window.draw(tile);

            // Highlight possible moves
            for (auto& move : possibleMoves) {
                if (move.x == x && move.y == y) {
                    tile.setFillColor(sf::Color::Green);
                    window.draw(tile);
                }
            }

            if (board[y][x]) {
                sf::Sprite sprite;
                sprite.setTexture(texture);
                sprite.setTextureRect(board[y][x]->getTextureRect());
                sprite.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                window.draw(sprite);
            }
        }
    }
}

// Converts mouse click to board coordinate
sf::Vector2i getBoardPosition(sf::Vector2i pixelPos) {
    return sf::Vector2i(pixelPos.x / TILE_SIZE, pixelPos.y / TILE_SIZE);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(TILE_SIZE * 8, TILE_SIZE * 8), "Chess Game with SFML");
    sf::Texture texture;
    if (!texture.loadFromFile("chess_pieces.png")) {
        std::cerr << "Failed to load piece texture!\n";
        return -1;
    }

    // Initialize board with a few pieces
    board[6][4] = new Piece(PieceType::PAWN, PieceColor::WHITE); // White pawn
    board[1][3] = new Piece(PieceType::PAWN, PieceColor::BLACK); // Black pawn

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i pos = getBoardPosition(sf::Mouse::getPosition(window));
                if (pos.x < 0 || pos.x >= 8 || pos.y < 0 || pos.y >= 8) continue;

                Piece* clickedPiece = board[pos.y][pos.x];

                // Handle valid move selection and piece movement
                if (selectedPiece && std::find(possibleMoves.begin(), possibleMoves.end(), pos) != possibleMoves.end()) {
                    board[pos.y][pos.x] = selectedPiece;
                    board[selectedPiece->position.y][selectedPiece->position.x] = nullptr;
                    selectedPiece->position = pos;
                    currentTurn = (currentTurn == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
                    selectedPiece = nullptr;
                    possibleMoves.clear();
                } else if (clickedPiece && clickedPiece->color == currentTurn) {
                    selectedPiece = clickedPiece;
                    possibleMoves.clear();
                    selectedPiece->position = pos;
                    getLegalMovesRecursive(board, selectedPiece, pos.x, pos.y, possibleMoves);
                }
            }
        }

        window.clear();
        drawBoard(window, texture);
        window.display();
    }

    // Free memory
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            delete board[y][x];

    return 0;
}
