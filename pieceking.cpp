#include "pieceking.hpp"

// Constructor
PieceKing::PieceKing(char colour) : Piece(colour) {
    mName = 'K';
    mColour != 'W' ? mSymbol = "\u2654" : mSymbol = "\u265A";
}

// Methods
bool PieceKing::legalPieceMove(int currentRank, int currentFile, int desiredPosition[]) {
    int dRank = desiredPosition[0] - currentRank;
    int dFile = desiredPosition[1] - currentFile;

    if (abs(dRank) > 1 || abs(dFile) > 1)
        return false;

    return true;
}