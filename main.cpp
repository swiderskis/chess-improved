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
    bool validInput = false;
    bool legalMove = false;

    char desiredPieceToMove = ' ';
    char turnChar = 'W';

    int turn = 0;  // indicates which player's turn it is, where white = 0 & black = 1
    int currentPosition[] = {-1, -1};
    int desiredPosition[] = {0, 0};

    const string ERROR_INVALID_INPUT = "This input is not valid!\n";
    const string ERROR_OWN_KING_IN_CHECK = "This puts your own king in check!\n";

    string playerInput = "";

    vector<string> moves;

    Piece* board[8][8];
    Piece* selectedPiece;
    Piece* destinationPiece;

    initialiseBoard(board);

    // Loop to handle each player move until the game is finished
    while (1) {
        clearConsole();
        printBoard(turn, board);

        if (kingInCheck(turnChar, board)) {
            turn == 0 ? cout << "White's " : cout << "Black's ";
            cout << "king is in check!\n";
        }

        // Loop to handle player inputting their desired move
        while (!legalMove) {
            turn == 0 ? cout << "White, " : cout << "Black, ";
            cout << "please input your move:\n";
            cin >> playerInput;

            validInput = processPlayerInput(playerInput, &desiredPieceToMove, currentPosition, desiredPosition, board);

            if (!validInput) {
                cout << ERROR_INVALID_INPUT;
                continue;
            }

            legalMove = checkingLegalMove(true, desiredPieceToMove, turnChar, currentPosition, desiredPosition, board);

            // Check if move puts own king in check
            if (legalMove) {
                selectedPiece = board[currentPosition[0]][currentPosition[1]];
                destinationPiece = board[desiredPosition[0]][desiredPosition[1]];

                board[currentPosition[0]][currentPosition[1]] = new Piece();
                board[desiredPosition[0]][desiredPosition[1]] = selectedPiece;

                legalMove = !kingInCheck(turnChar, board);

                if (!legalMove) {
                    cout << ERROR_OWN_KING_IN_CHECK;

                    board[currentPosition[0]][currentPosition[1]] = selectedPiece;
                    board[desiredPosition[0]][desiredPosition[1]] = destinationPiece;
                } else
                    board[desiredPosition[0]][desiredPosition[1]]->setHasMoved();
            }
        }

        moves.push_back(playerInput);
        legalMove = false;

        turn = 1 - turn;
        turn == 0 ? turnChar = 'W' : turnChar = 'B';
    }

    return 0;
}