#include "piecequeen.hpp"

// Constructor
PieceQueen::PieceQueen(char colour) : Piece(colour) {
    mName = 'Q';
    mColour != 'W' ? mSymbol = "\u2655" : mSymbol = "\u265B";
}

// Methods
bool PieceQueen::legalPieceMove(int currentRank, int currentFile, int desiredPosition[]) {
    int dRank = desiredPosition[0] - currentRank;
    int dFile = desiredPosition[1] - currentFile;

    if (dRank != 0 && dFile != 0 && abs(dRank) != abs(dFile))
        return false;

    return true;
}