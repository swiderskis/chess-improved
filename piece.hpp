#ifndef PIECE_HPP
#define PIECE_HPP

#include <cmath>
#include <string>

using std::abs, std::string;

class Piece {
   protected:
    bool mCanEnPassant;  // if piece can be captured by en passant
    bool mHasMoved;

    char mColour;
    char mName;

    string mSymbol;

   public:
    // Constructors
    Piece();

    Piece(char colour);

    // Methods
    bool getHasMoved();
    bool getCanEnPassant();

    virtual bool legalPieceMove(int currentRank, int currentFile, int desiredPosition[]);

    char getColour();
    char getName();

    string getSymbol();

    void setCanEnPassant(bool canEnPassant);
    void setHasMoved();
};

#endif