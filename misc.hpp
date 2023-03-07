#ifndef MISC_HPP
#define MISC_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include "piece.hpp"

using std::cout, std::ifstream, std::ofstream, std::ostream_iterator, std::string, std::vector, std::begin, std::copy, std::end, std::find, std::system;

// Takes the player's input and interprets the intended move
bool processPlayerInput(char turn, char* pDesiredPieceToMove, char* pPromotedPiece, int currentPosition[2], int desiredPosition[2], string playerInput, Piece* board[8][8]);

// Converts an integer stored as a char to an int
int charToInt(char character);

// Converts a lowercase letter stored as a char to an int, where a = 0, b = 1, etc...
int letterToInt(char character);

// Clears console
void clearConsole();

// Saves current game to PGN.txt file
void saveGame(vector<string> moves);

// Loads game from PGN.txt file
vector<string> loadGame();

#endif