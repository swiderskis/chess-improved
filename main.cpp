#include <iostream>
#include <string>
#include <vector>

#include "board.hpp"
#include "misc.hpp"
#include "piece.hpp"
#include "piecebishop.hpp"
#include "pieceking.hpp"
#include "pieceknight.hpp"
#include "piecepawn.hpp"
#include "piecequeen.hpp"
#include "piecerook.hpp"

using std::cout, std::cin, std::string, std::vector;

int main() {
    bool check = false;
    bool checkmate = false;
    bool enPassant = false;
    bool putSelfInCheck = false;
    bool validInput = false;

    char desiredPieceToMove = ' ';
    char promotionPiece = ' ';
    char turnChar = 'W';

    int legalMoveCount = 0;
    int turn = 0;  // indicates which player's turn it is, where white = 0 & black = 1
    int currentPosition[2] = {-1, -1};
    int desiredPosition[2] = {-1, -1};

    const string ERROR_AMBIGUOUS_MOVE = "This move is ambiguous!\n";
    const string ERROR_CANNOT_CASTLE_WHILE_IN_CHECK = "You cannot castle while in check!\n";
    const string ERROR_CANNOT_PROMOTE_PIECE = "You cannot promote a piece!\n";
    const string ERROR_ILLEGAL_MOVE = "This is not a legal move!\n";
    const string ERROR_INVALID_INPUT = "This input is not valid!\n";
    const string ERROR_MUST_SPECIFY_PROMOTION_PIECE = "You must specify a promotion piece!\n";
    const string ERROR_OWN_KING_STILL_IN_CHECK = "Your king is still in check!\n";
    const string ERROR_PUTS_OWN_KING_IN_CHECK = "This puts your own king in check!\n";

    string playerInput = "";

    vector<string> moves;

    Piece* selectedPiece;
    Piece* destinationPiece;
    Piece* board[8][8];

    initialiseBoard(board);

    // Loop to handle each player move until the game is finished
    while (1) {
        clearConsole();
        printBoard(turn, board);

        check = kingInCheck(turnChar, board);

        if (check)
            checkmate = kingIsCheckmated(turnChar, board);

        if (checkmate)
            break;

        if (check) {
            turn == 0 ? cout << "White's " : cout << "Black's ";
            cout << "king is in check!\n";
        }

        // Loop to handle player inputting their desired move
        while (legalMoveCount != 1) {
            turn == 0 ? cout << "White, " : cout << "Black, ";
            cout << "please input your move:\n";
            cin >> playerInput;

            // Checks if input is valid
            validInput = processPlayerInput(turnChar, &desiredPieceToMove, &promotionPiece, currentPosition, desiredPosition, playerInput, board);

            if (!validInput) {
                cout << ERROR_INVALID_INPUT;
                continue;
            }

            // Handles castling
            if (playerInput.find("O-O") != string::npos) {
                if (check) {
                    cout << ERROR_CANNOT_CASTLE_WHILE_IN_CHECK;
                    continue;
                }

                if (!castlingValid(turnChar, currentPosition, desiredPosition, board))
                    continue;

                legalMoveCount = 1;
            }

            // Prevents promoting for non-pawns
            if (desiredPieceToMove != 'P' && promotionPiece != ' ') {
                cout << ERROR_CANNOT_PROMOTE_PIECE;
                continue;
            }

            // Ensures promotion piece is specified if pawn reaches last rank
            if (desiredPieceToMove == 'P' && promotionPiece == ' ')
                if ((turnChar == 'W' && desiredPosition[0] == 7) || (turnChar == 'B' && desiredPosition[0] == 0)) {
                    cout << ERROR_MUST_SPECIFY_PROMOTION_PIECE;
                    continue;
                }

            // Ensures move is legal and unambiguous
            if (playerInput.find("O-O") == string::npos)  // skip if castling (handled above)
                legalMoveCount = checkingLegalMove(desiredPieceToMove, turnChar, currentPosition, desiredPosition, board);

            // Ensures en passant move is valid
            if (desiredPieceToMove == 'P' && currentPosition[1] != desiredPosition[1] && board[desiredPosition[0]][desiredPosition[1]]->getColour() == 'N')
                enPassant = true;
            else
                enPassant = false;

            if (legalMoveCount == 0) {
                cout << ERROR_ILLEGAL_MOVE;
                continue;
            }

            if (legalMoveCount > 1) {
                cout << ERROR_AMBIGUOUS_MOVE;
                continue;
            }

            // Updates board state
            selectedPiece = board[currentPosition[0]][currentPosition[1]];

            if (enPassant) {
                destinationPiece = board[currentPosition[0]][desiredPosition[1]];
                board[currentPosition[0]][desiredPosition[1]] = new Piece();
            } else
                destinationPiece = board[desiredPosition[0]][desiredPosition[1]];

            board[currentPosition[0]][currentPosition[1]] = new Piece();

            switch (promotionPiece) {
                case 'Q':
                    board[desiredPosition[0]][desiredPosition[1]] = new PieceQueen(turnChar);
                    break;

                case 'R':
                    board[desiredPosition[0]][desiredPosition[1]] = new PieceRook(turnChar);
                    break;

                case 'B':
                    board[desiredPosition[0]][desiredPosition[1]] = new PieceBishop(turnChar);
                    break;

                case 'N':
                    board[desiredPosition[0]][desiredPosition[1]] = new PieceKnight(turnChar);
                    break;

                default:
                    board[desiredPosition[0]][desiredPosition[1]] = selectedPiece;
                    break;
            }

            promotionPiece = ' ';

            // Checks if move has put own king in check
            putSelfInCheck = kingInCheck(turnChar, board);

            if (putSelfInCheck) {
                if (check)
                    cout << ERROR_OWN_KING_STILL_IN_CHECK;
                else
                    cout << ERROR_PUTS_OWN_KING_IN_CHECK;

                board[currentPosition[0]][currentPosition[1]] = selectedPiece;

                if (enPassant) {
                    board[currentPosition[0]][desiredPosition[1]] = destinationPiece;
                    board[desiredPosition[0]][desiredPosition[1]] = new Piece();
                } else
                    board[desiredPosition[0]][desiredPosition[1]] = destinationPiece;

                legalMoveCount = 0;

                continue;
            }

            board[desiredPosition[0]][desiredPosition[1]]->setHasMoved();

            // Sets pawn can be captured by en passant if it has pushed forward 2 squares
            if (abs(desiredPosition[0] - currentPosition[0]) == 2 && board[desiredPosition[0]][desiredPosition[1]]->getName() == 'P')
                board[desiredPosition[0]][desiredPosition[1]]->setCanEnPassant(true);

            // Moves the rook if castling
            if (playerInput.find("O-O") == string::npos)
                continue;

            if (desiredPosition[1] == 2) {
                board[currentPosition[0]][3] = board[currentPosition[0]][0];
                board[currentPosition[0]][0] = new Piece();
                board[currentPosition[0]][3]->setHasMoved();
            }

            if (desiredPosition[1] == 6) {
                board[currentPosition[0]][5] = board[currentPosition[0]][7];
                board[currentPosition[0]][7] = new Piece();
                board[currentPosition[0]][5]->setHasMoved();
            }
        }

        legalMoveCount = 0;

        moves.push_back(playerInput);

        turn = 1 - turn;
        turn == 0 ? turnChar = 'W' : turnChar = 'B';

        // Sets pawns can't be captured by en passant if they pushed forward 2 squares last turn
        for (int rank = 0; rank < 8; rank++) {
            for (int file = 0; file < 8; file++) {
                if (board[rank][file]->getColour() != turnChar)
                    continue;

                board[rank][file]->setCanEnPassant(false);
            }
        }
    }

    if (checkmate) {
        turn == 0 ? moves.push_back("0-1") : moves.push_back("1-0");

        turn == 0 ? cout << "Black " : cout << "White ";
        cout << "wins!\n";
    }

    return 0;
}