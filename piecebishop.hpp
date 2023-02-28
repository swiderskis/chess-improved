#ifndef PIECEBISHOP_HPP
#define PIECEBISHOP_HPP

#include "piece.hpp"

class PieceBishop : public Piece {
   public:
    // Constructor
    PieceBishop(char colour);

    // Methods
    bool legalPieceMove(int currentRank, int currentFile, int desiredPosition[]);
};

#endif