#ifndef PIECEROOK_HPP
#define PIECEROOK_HPP

#include "piece.hpp"

class PieceRook : public Piece {
   public:
    // Constructor
    PieceRook(char colour);

    // Methods
    bool legalPieceMove(int currentRank, int currentFile, int desiredPosition[]);
};

#endif