#include "piecebishop.hpp"

// Constructor
PieceBishop::PieceBishop(char colour) : Piece(colour) {
    mName = 'B';
    mColour != 'W' ? mSymbol = "\u2657" : mSymbol = "\u265D";
}

bool PieceBishop::legalPieceMove(int currentRank, int currentFile, int desiredPosition[]) {
    int dRank = desiredPosition[0] - currentRank;
    int dFile = desiredPosition[1] - currentFile;

    if (abs(dRank) != abs(dFile))
        return false;

    return true;
}