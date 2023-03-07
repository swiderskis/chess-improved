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

// Checks if castling is valid
bool castlingValid(char turn, int currentPosition[2], int desiredPosition[2], Piece* board[8][8]);

// Checks if player has legal move, used for checkmate / stalemate checks
bool hasLegalMove(char turn, Piece* board[8][8]);

// Checks if the king is in check
bool kingInCheck(char turn, Piece* board[8][8]);

// Checks if a move is legal in the context of the board (i.e. piece is not obsctructed, pawn is capturing / pushing correctly)
bool legalBoardMove(char desiredPieceToMove, char turn, int currentPosition[2], int desiredPosition[2], Piece* board[8][8]);

// Checks the input move is legal and unambiguous
int checkingLegalMove(char desiredPieceToMove, char turn, int currentPosition[2], int desiredPosition[2], Piece* board[8][8]);

// Initialises board
void initialiseBoard(Piece* board[8][8]);

// Prints the current board state, flipping it based on the current player's turn
void printBoard(char turn, Piece* board[8][8]);

#endif