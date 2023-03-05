#include "board.hpp"

// Checks if the king is in check
bool kingInCheck(char turnChar, Piece* board[8][8]) {
    char opponentColour = ' ';

    int hasKingInCheckCount = 0;
    int kingPos[2] = {-1, -1};
    int opponentPos[2] = {-1, -1};

    turnChar == 'W' ? opponentColour = 'B' : opponentColour = 'W';

    // Finds position of king
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if (board[rank][file]->getName() != 'K')
                continue;

            if (board[rank][file]->getColour() != turnChar)
                continue;

            kingPos[0] = rank;
            kingPos[1] = file;
            break;
        }

        // Breaks out of second for loop once king position found
        if (kingPos[0] != -1 && kingPos[1] != -1)
            break;
    }

    // Sees if any opponent piece can move to the king's square
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if (board[rank][file]->getColour() != opponentColour)
                continue;

            opponentPos[0] = rank;
            opponentPos[1] = file;

            hasKingInCheckCount = checkingLegalMove(board[rank][file]->getName(), opponentColour, opponentPos, kingPos, board);

            if (hasKingInCheckCount > 0)
                return true;
        }
    }

    return false;
}

// Checks if the king has been checkmated
bool kingIsCheckmated(char turnChar, Piece* board[8][8]) {
    bool check = false;

    int legalMoveCount = 0;
    int currentPosition[] = {-1, -1};
    int desiredPosition[] = {-1, -1};

    Piece* selectedPiece;
    Piece* destinationPiece;

    // Finds a piece that belongs to player
    for (int rankCurr = 0; rankCurr < 8; rankCurr++) {
        for (int fileCurr = 0; fileCurr < 8; fileCurr++) {
            if (board[rankCurr][fileCurr]->getColour() != turnChar)
                continue;

            currentPosition[0] = rankCurr;
            currentPosition[1] = fileCurr;

            // Finds a legal move for that piece and sees if it takes the king out of check
            for (int rankNew = 0; rankNew < 8; rankNew++) {
                for (int fileNew = 0; fileNew < 8; fileNew++) {
                    desiredPosition[0] = rankNew;
                    desiredPosition[1] = fileNew;

                    legalMoveCount = checkingLegalMove(board[rankCurr][fileCurr]->getName(), turnChar, currentPosition, desiredPosition, board);

                    if (legalMoveCount != 1)
                        continue;

                    selectedPiece = board[rankCurr][fileCurr];
                    destinationPiece = board[rankNew][fileNew];

                    board[rankCurr][fileCurr] = new Piece();
                    board[rankNew][fileNew] = selectedPiece;

                    check = kingInCheck(turnChar, board);

                    board[rankCurr][fileCurr] = selectedPiece;
                    board[rankNew][fileNew] = destinationPiece;

                    if (!check)
                        return false;
                }
            }
        }
    }

    return true;
}

// Checks if a move is legal in the context of the board (i.e. piece is not obsctructed, pawn is capturing / pushing correctly)
bool legalBoardMove(char desiredPieceToMove, char turnChar, int currentPosition[2], int desiredPosition[2], Piece* board[8][8]) {
    int dRank = desiredPosition[0] - currentPosition[0];
    int dFile = desiredPosition[1] - currentPosition[1];

    // Checks if the desired position is occupied by a piece of the same colour
    if (board[desiredPosition[0]][desiredPosition[1]]->getColour() == turnChar)
        return false;

    // Checks pawn move to sees it is capturing / pushing correctly
    if (board[currentPosition[0]][currentPosition[1]]->getName() == 'P') {
        if (dFile != 0 && board[desiredPosition[0]][desiredPosition[1]]->getColour() == 'N')
            return false;

        if (dFile == 0 && board[desiredPosition[0]][desiredPosition[1]]->getColour() != 'N')
            return false;
    }

    if (dRank > 0)
        dRank--;
    else if (dRank < 0)
        dRank++;

    if (dFile > 0)
        dFile--;
    else if (dFile < 0)
        dFile++;

    // Skips obstruction check if piece is a knight
    if (board[currentPosition[0]][currentPosition[1]]->getName() == 'N')
        return true;

    // Checks if piece is obstructed
    while (dRank != 0 || dFile != 0) {
        if (board[currentPosition[0] + dRank][currentPosition[1] + dFile]->getColour() != 'N')
            return false;

        if (dRank > 0)
            dRank--;
        else if (dRank < 0)
            dRank++;

        if (dFile > 0)
            dFile--;
        else if (dFile < 0)
            dFile++;
    }

    return true;
}

// Checks the input move is legal and unambiguous
int checkingLegalMove(char desiredPieceToMove, char turnChar, int currentPosition[2], int desiredPosition[2], Piece* board[8][8]) {
    bool pieceMoveIsLegal = false;
    bool boardMoveIsLegal = false;

    int rank = 0;
    int rankMax = 8;
    int file = 0;
    int fileMax = 8;
    int legalMoveCount = 0;

    int tempCurrentPosition[2] = {-1, -1};

    // If current position was specified in player input, this narrows the scan range in the upcoming for loops
    // Required for both performance and handling potential ambiguous moves
    if (currentPosition[0] != -1) {
        rank = currentPosition[0];
        rankMax = rank + 1;
    }

    if (currentPosition[1] != -1) {
        file = currentPosition[1];
        fileMax = file + 1;
    }

    // Scans the board to find the desired piece to move, and determines if this move is legal
    for (int rankCurr = rank; rankCurr < rankMax; rankCurr++) {
        for (int fileCurr = file; fileCurr < fileMax; fileCurr++) {
            if (board[rankCurr][fileCurr]->getName() != desiredPieceToMove)
                continue;

            if (board[rankCurr][fileCurr]->getColour() != turnChar)
                continue;

            pieceMoveIsLegal = board[rankCurr][fileCurr]->legalPieceMove(rankCurr, fileCurr, desiredPosition);

            if (!pieceMoveIsLegal)
                continue;

            tempCurrentPosition[0] = rankCurr;
            tempCurrentPosition[1] = fileCurr;

            boardMoveIsLegal = legalBoardMove(desiredPieceToMove, turnChar, tempCurrentPosition, desiredPosition, board);

            if (!boardMoveIsLegal)
                continue;

            currentPosition[0] = rankCurr;
            currentPosition[1] = fileCurr;

            legalMoveCount++;
        }
    }

    return legalMoveCount;
}

// Initialises board
void initialiseBoard(Piece* board[8][8]) {
#if 0
    // Initialise empty board spaces
    for (int rank = 2; rank < 7; rank++) {
        for (int file = 0; file < 8; file++) {
            board[rank][file] = new Piece();
        }
    }

    // Initialise pawns
    for (int file = 0; file < 8; file++)
        board[1][file] = new PiecePawn('W');

    for (int file = 0; file < 8; file++)
        board[6][file] = new PiecePawn('B');

    // Initialise white pieces
    board[0][0] = new PieceRook('W');
    board[0][1] = new PieceKnight('W');
    board[0][2] = new PieceBishop('W');
    board[0][3] = new PieceQueen('W');
    board[0][4] = new PieceKing('W');
    board[0][5] = new PieceBishop('W');
    board[0][6] = new PieceKnight('W');
    board[0][7] = new PieceRook('W');

    // Initialise black pieces
    board[7][0] = new PieceRook('B');
    board[7][1] = new PieceKnight('B');
    board[7][2] = new PieceBishop('B');
    board[7][3] = new PieceQueen('B');
    board[7][4] = new PieceKing('B');
    board[7][5] = new PieceBishop('B');
    board[7][6] = new PieceKnight('B');
    board[7][7] = new PieceRook('B');

#else
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            board[rank][file] = new Piece();
        }
    }

    board[0][0] = new PieceKing('W');
    board[7][7] = new PieceKing('B');
    board[1][7] = new PiecePawn('B');

#endif
}

// Prints the current board state, flipping it based on the current player's turn
void printBoard(int turn, Piece* board[8][8]) {
    cout << "\n";

    if (turn == 0) {
        for (int rank = 7; rank >= 0; rank--) {
            cout << " " << rank + 1 << "  ";

            for (int file = 0; file < 8; file++) {
                cout << " | " << board[rank][file]->getSymbol();
            }

            cout << "\n---  +---+---+---+---+---+---+---+---\n";
        }

        cout << "\n     | a | b | c | d | e | f | g | h\n\n";

    } else {
        for (int rank = 0; rank < 8; rank++) {
            cout << " " << rank + 1 << "  ";

            for (int file = 7; file >= 0; file--) {
                cout << " | " << board[rank][file]->getSymbol();
            }

            cout << "\n---  +---+---+---+---+---+---+---+---\n";
        }

        cout << "\n     | h | g | f | e | d | c | b | a\n\n";
    }
}