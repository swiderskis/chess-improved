#ifndef PIECEQUEEN_HPP
#define PIECEQUEEN_HPP

#include "piece.hpp"

class PieceQueen : public Piece {
   public:
    // Constructor
    PieceQueen(char colour);

    // Methods
    bool legalPieceMove(int currentRank, int currentFile, int desiredPosition[]);
};

#endif