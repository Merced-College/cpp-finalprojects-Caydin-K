// ChessGame.cpp
// Caydin Klups 
// A simplified chess game using SFML with recursive move generation and basic interactivity

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Piece.h"
#include "Logic.h"

const int TILE_SIZE = 80;        // Size of each tile in pixels
const int BOARD_SIZE = 8;        // Board is 8x8 like a standard chessboard

// Chessboard represented as a 2D array of Piece pointers
Piece* board[BOARD_SIZE][BOARD_SIZE] = {nullptr};

// Track which player's turn it is (White starts)
PieceColor currentTurn = PieceColor::WHITE;

// Store the currently selected piece and its valid moves
Piece* selectedPiece = nullptr;
std::vector<sf::Vector2i> possibleMoves;

// Draws the board, highlights, and pieces
void drawBoard(sf::RenderWindow& window, sf::Texture& texture) {
    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE)); // Define tile shape

    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            // Alternate tile colors for checkerboard pattern
            bool isWhite = (x + y) % 2 == 0;
            tile.setFillColor(isWhite ? sf::Color::White : sf::Color(128, 128, 128));
            tile.setPosition(x * TILE_SIZE, y * TILE_SIZE);
            window.draw(tile);

            // Highlight possible moves with green
            for (auto& move : possibleMoves) {
                if (move.x == x && move.y == y) {
                    tile.setFillColor(sf::Color::Green);
                    window.draw(tile);
                }
            }

            // If there’s a piece on this tile, draw it
            if (board[y][x]) {
                sf::Sprite sprite;
                sprite.setTexture(texture);                                       // Set sprite texture
                sprite.setTextureRect(board[y][x]->getTextureRect());            // Choose the right image region
                sprite.setPosition(x * TILE_SIZE, y * TILE_SIZE);                // Set position on board
                window.draw(sprite);                                             // Draw the sprite
            }
        }
    }
}

// Converts a pixel position from mouse click into board (x, y) position
sf::Vector2i getBoardPosition(sf::Vector2i pixelPos) {
    return sf::Vector2i(pixelPos.x / TILE_SIZE, pixelPos.y / TILE_SIZE);
}

int main() {
    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(TILE_SIZE * 8, TILE_SIZE * 8), "Chess Game with SFML");

    // Load the texture that contains the chess piece sprites
    sf::Texture texture;
    if (!texture.loadFromFile("chess_pieces.png")) {
        std::cerr << "Failed to load piece texture!\n";
        return -1;
    }

    // Place two pawns for testing: one white and one black
    board[6][4] = new Piece(PieceType::PAWN, PieceColor::WHITE); // White pawn
    board[1][3] = new Piece(PieceType::PAWN, PieceColor::BLACK); // Black pawn

    // Main game loop - continues running while the window is open
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close(); // Close window event
            else if (event.type == sf::Event::MouseButtonPressed) {
                // Get the board coordinates from the mouse click
                sf::Vector2i pos = getBoardPosition(sf::Mouse::getPosition(window));

                // Ensure click is inside the board boundaries
                if (pos.x < 0 || pos.x >= 8 || pos.y < 0 || pos.y >= 8) continue;

                // Get the piece on the clicked tile
                Piece* clickedPiece = board[pos.y][pos.x];

                // === LINE 74 EXPLAINED ===
                // If a piece is already selected and the clicked position is a valid move,
                // move the piece to the clicked position and clear the selection.
                if (selectedPiece && std::find(possibleMoves.begin(), possibleMoves.end(), pos) != possibleMoves.end()) {
                    board[pos.y][pos.x] = selectedPiece;                                // Move the selected piece
                    board[selectedPiece->position.y][selectedPiece->position.x] = nullptr; // Clear old spot
                    selectedPiece->position = pos;                                      // Update internal position
                    currentTurn = (currentTurn == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE; // Switch turns
                    selectedPiece = nullptr;                                            // Deselect
                    possibleMoves.clear();                                              // Clear highlights
                } 
                // If a piece is clicked that belongs to the current player, select it
                else if (clickedPiece && clickedPiece->color == currentTurn) {
                    selectedPiece = clickedPiece;                                       // Set selected piece
                    possibleMoves.clear();                                              // Clear any previous moves
                    selectedPiece->position = pos;                                      // Store its position
                    getLegalMovesRecursive(board, selectedPiece, pos.x, pos.y, possibleMoves); // Generate moves
                }
            }
        }

        // Clear and redraw everything
        window.clear();
        drawBoard(window, texture);
        window.display();
    }

    // Cleanup: Free all dynamically allocated pieces
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            delete board[y][x];

    return 0;
}
