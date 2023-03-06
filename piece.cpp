#include "piece.hpp"

// Constructor
Piece::Piece() : mHasMoved(false), mCanEnPassant(false), mColour('N'), mName('_'), mSymbol(" ") {}

// Constructor used for inherited piece classes
Piece::Piece(char colour) : mHasMoved(false), mCanEnPassant(false), mColour(colour) {}

// Methods
bool Piece::getHasMoved() {
    return mHasMoved;
}

bool Piece::getCanEnPassant() {
    return mCanEnPassant;
}

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

void Piece::setCanEnPassant(bool canEnPassant) {
    if (mName == 'P')
        mCanEnPassant = canEnPassant;
    else
        mCanEnPassant = false;
}

void Piece::setHasMoved() {
    mHasMoved = true;
}