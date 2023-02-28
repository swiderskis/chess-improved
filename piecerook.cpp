#include "piecerook.hpp"

// Constructor
PieceRook::PieceRook(char colour) : Piece(colour) {
    mName = 'R';
    mColour != 'W' ? mSymbol = "\u2656" : mSymbol = "\u265C";
}

// Methods
bool PieceRook::legalPieceMove(int currentRank, int currentFile, int desiredPosition[]) {
    int dRank = desiredPosition[0] - currentRank;
    int dFile = desiredPosition[1] - currentFile;

    if (dRank != 0 && dFile != 0)
        return false;

    return true;
}