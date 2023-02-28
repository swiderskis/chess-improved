#include "piecepawn.hpp"

// Constructor
PiecePawn::PiecePawn(char colour) : Piece(colour) {
    mName = 'P';
    mColour != 'W' ? mSymbol = "\u2659" : mSymbol = "\u265F";
}

// Methods
// Note that this method does not check if the pawn is capturing - this is done in legalBoardMove function later on
bool PiecePawn::legalPieceMove(int currentRank, int currentFile, int desiredPosition[]) {
    int dRank = desiredPosition[0] - currentRank;
    int dFile = desiredPosition[1] - currentFile;

    if (abs(dRank) > 2 || abs(dFile) > 1)
        return false;

    if (((mColour == 'W' && dRank == 2) || (mColour == 'B' && dRank == -2)) && (mHasMoved == 1 || dFile != 0))
        return false;

    if ((mColour == 'W' && dRank != 1 && dRank != 2) || (mColour == 'B' && dRank != -1 && dRank != -2)) {
        return false;
    }

    return true;
}