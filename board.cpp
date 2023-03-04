#include "board.hpp"

// Checks if the king is in check
bool kingInCheck(char turnChar, Piece* board[8][8]) {
    bool hasKingInCheck = false;

    char opponentColour = ' ';

    int kingPos[2] = {-1, -1};
    int opponentPos[2] = {-1, -1};

    turnChar == 'W' ? opponentColour = 'B' : opponentColour = 'W';

    // Find position of king
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if (board[rank][file]->getName() == 'K' && board[rank][file]->getColour() == turnChar) {
                kingPos[0] = rank;
                kingPos[1] = file;
                break;
            }
        }

        // Breaks out of second for loop once king position found
        if (kingPos[0] != -1 && kingPos[1] != -1)
            break;
    }

    // See if any opponent piece can move to the king's square
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if (board[rank][file]->getColour() != turnChar && board[rank][file]->getColour() != 'N') {
                opponentPos[0] = rank;
                opponentPos[1] = file;

                hasKingInCheck = checkingLegalMove(false, board[rank][file]->getName(), opponentColour, opponentPos, kingPos, board);
            }

            if (hasKingInCheck)
                return true;
        }
    }

    return false;
}

// Checks if the king has been checkmated
bool kingIsCheckmated() {
    return false;
}

// Checks the input move is legal and unambiguous
bool checkingLegalMove(bool printError, char desiredPieceToMove, char turnChar, int currentPosition[2], int desiredPosition[2], Piece* board[8][8]) {
    bool pieceMoveIsLegal = false;
    bool boardMoveIsLegal = false;

    int rank = 0;
    int rankMax = 8;
    int file = 0;
    int fileMax = 8;
    int legalMoveCount = 0;

    int tempCurrentPosition[2] = {-1, -1};

    const string ERROR_AMBIGUOUS_MOVE = "This move is ambiguous!\n";
    const string ERROR_ILLEGAL_MOVE = "This is not a legal move!\n";

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
            if (board[rankCurr][fileCurr]->getName() == desiredPieceToMove && board[rankCurr][fileCurr]->getColour() == turnChar) {
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
    }

    if (legalMoveCount != 1) {
        if (legalMoveCount == 0 && printError)
            cout << ERROR_ILLEGAL_MOVE;
        else if (printError)
            cout << ERROR_AMBIGUOUS_MOVE;

        return false;
    }

    return true;
}

// Checks if a move is legal in the context of the board (i.e. piece is not blocked, move does not put king in check)
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

    // Checks if piece is obstructed
    if (board[currentPosition[0]][currentPosition[1]]->getName() != 'N') {
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
    }

    return true;
}

// Initialises board
void initialiseBoard(Piece* board[8][8]) {
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