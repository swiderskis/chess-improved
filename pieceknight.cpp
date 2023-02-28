#include "pieceknight.hpp"

// Constructor
PieceKnight::PieceKnight(char colour) : Piece(colour) {
    mName = 'N';
    mColour != 'W' ? mSymbol = "\u2658" : mSymbol = "\u265E";
}

// Methods

bool PieceKnight::legalPieceMove(int currentRank, int currentFile, int desiredPosition[]) {
    int dRank = desiredPosition[0] - currentRank;
    int dFile = desiredPosition[1] - currentFile;

    if (abs(dRank) == 2 && abs(dFile) == 1)
        return true;

    if (abs(dRank) == 1 && abs(dFile) == 2)
        return true;

    return false;
}