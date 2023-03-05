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
    bool putSelfInCheck = false;
    bool validInput = false;

    char desiredPieceToMove = ' ';
    char turnChar = 'W';

    int legalMoveCount = 0;
    int turn = 0;  // indicates which player's turn it is, where white = 0 & black = 1
    int currentPosition[] = {-1, -1};
    int desiredPosition[] = {0, 0};

    const string ERROR_AMBIGUOUS_MOVE = "This move is ambiguous!\n";
    const string ERROR_ILLEGAL_MOVE = "This is not a legal move!\n";
    const string ERROR_INVALID_INPUT = "This input is not valid!\n";
    const string ERROR_OWN_KING_STILL_IN_CHECK = "Your own king is still in check!\n";
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
            validInput = processPlayerInput(playerInput, &desiredPieceToMove, currentPosition, desiredPosition, board);

            if (!validInput) {
                cout << ERROR_INVALID_INPUT;
                continue;
            }

            // Ensures move is legal and unambiguous
            legalMoveCount = checkingLegalMove(desiredPieceToMove, turnChar, currentPosition, desiredPosition, board);

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
            destinationPiece = board[desiredPosition[0]][desiredPosition[1]];

            board[currentPosition[0]][currentPosition[1]] = new Piece();
            board[desiredPosition[0]][desiredPosition[1]] = selectedPiece;

            // Checks if move has put own king in check
            putSelfInCheck = kingInCheck(turnChar, board);

            if (putSelfInCheck) {
                if (check)
                    cout << ERROR_OWN_KING_STILL_IN_CHECK;
                else
                    cout << ERROR_PUTS_OWN_KING_IN_CHECK;

                board[currentPosition[0]][currentPosition[1]] = selectedPiece;
                board[desiredPosition[0]][desiredPosition[1]] = destinationPiece;

                legalMoveCount = 0;

                continue;
            }

            board[desiredPosition[0]][desiredPosition[1]]->setHasMoved();
        }

        moves.push_back(playerInput);
        legalMoveCount = 0;

        turn = 1 - turn;
        turn == 0 ? turnChar = 'W' : turnChar = 'B';
    }

    if (checkmate) {
        turn == 'W' ? moves.push_back("0-1") : moves.push_back("1-0");

        turn == 'W' ? cout << "White " : cout << "Black ";
        cout << "wins!\n";

        cin >> playerInput;
    }

    return 0;
}