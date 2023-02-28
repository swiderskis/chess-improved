#include "piece.hpp"

// Constructor
Piece::Piece() : mHasMoved(false), mColour('N'), mName('_'), mSymbol(" ") {}

// Constructor used for inherited piece classes
Piece::Piece(char colour) : mHasMoved(false), mColour(colour) {}

// Methods
bool Piece::legalPieceMove(int currentRank, int currentFile, int desiredPosition[]) {
    return false;
}

char Piece::getColour() {
    return mColour;
}

char Piece::getName() {
    return mName;
}

string Piece::getSymbol() {
    return mSymbol;
}

void Piece::setHasMoved() {
    mHasMoved = true;
}