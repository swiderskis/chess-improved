#ifndef PIECEKNIGHT_HPP
#define PIECEKNIGHT_HPP

#include "piece.hpp"

class PieceKnight : public Piece {
   public:
    // Constructor
    PieceKnight(char colour);

    // Methods
    bool legalPieceMove(int currentRank, int currentFile, int desiredPosition[]);
};

#endif