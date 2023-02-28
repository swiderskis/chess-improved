#ifndef PIECEPAWN_HPP
#define PIECEPAWN_HPP

#include "piece.hpp"

class PiecePawn : public Piece {
   public:
    // Constructor
    PiecePawn(char colour);

    // Methods
    bool legalPieceMove(int currentRank, int CurrentFile, int desiredPositon[]);
};

#endif