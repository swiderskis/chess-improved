#ifndef PIECEKING_HPP
#define PIECEKING_HPP

#include "piece.hpp"

class PieceKing : public Piece {
   public:
    // Constructor
    PieceKing(char colour);

    // Methods
    bool legalPieceMove(int currentRank, int currentFile, int desiredPosition[]);
};

#endif