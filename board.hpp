#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>

#include "piece.hpp"
#include "piecebishop.hpp"
#include "pieceking.hpp"
#include "pieceknight.hpp"
#include "piecepawn.hpp"
#include "piecequeen.hpp"
#include "piecerook.hpp"

using std::cout;

// Checks if the king is in check
bool kingInCheck(char turnChar, Piece* board[8][8]);

// Checks if the king has been checkmated
bool kingIsCheckmated();

// Checks the input move is legal and unambiguous
bool checkingLegalMove(bool printError, char desiredPieceToMove, char turnChar, int currentPosition[2], int desiredPosition[2], Piece* board[8][8]);

// Checks if a move is legal in the context of the board (i.e. piece is not blocked, move does not put king in check)
bool legalBoardMove(char desiredPieceToMove, char turnChar, int currentPosition[2], int desiredPosition[2], Piece* board[8][8]);

// Initialises board
void initialiseBoard(Piece* board[8][8]);

// Prints the current board state, flipping it based on the current player's turn
void printBoard(int turn, Piece* board[8][8]);

#endif