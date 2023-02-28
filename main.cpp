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

    int turn = 0;  // indicates which player's turn it is, where white = 0 & black = 1
    int currentPosition[] = {-1, -1};
    int desiredPosition[] = {0, 0};

    const string ERROR_INVALID_INPUT = "This input is not valid!\n";

    string playerInput = "";

    vector<string> moves;

    Piece* board[8][8];

    initialiseBoard(board);

    // Loop to handle each player move until the game is finished
    while (1) {
        clearConsole();
        printBoard(turn, board);

        // Loop to handle player inputting their desired move
        while (legalMove == false) {
            turn == 0 ? cout << "White, " : cout << "Black, ";
            cout << "please input your move:\n";
            cin >> playerInput;

            validInput = processPlayerInput(playerInput, &desiredPieceToMove, currentPosition, desiredPosition, board);

            if (validInput == false) {
                cout << ERROR_INVALID_INPUT;
                continue;
            }

            legalMove = checkingLegalMove(desiredPieceToMove, turn, currentPosition, desiredPosition, board);
        }

        board[desiredPosition[0]][desiredPosition[1]] = board[currentPosition[0]][currentPosition[1]];
        board[desiredPosition[0]][desiredPosition[1]]->setHasMoved();

        board[currentPosition[0]][currentPosition[1]] = new Piece();

        moves.push_back(playerInput);
        legalMove = false;

        turn = 1 - turn;
    }

    return 0;
}