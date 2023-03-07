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

using std::cout, std::cin, std::string, std::vector, std::to_string;

int main() {
    bool check = false;
    bool checkmate = false;
    bool drawAgreed = false;
    bool enPassant = false;
    bool failedLoad = false;
    bool loadingGame = false;
    bool putSelfInCheck = false;
    bool resign = false;
    bool stalemate = false;
    bool validInput = false;

    char desiredPieceToMove = ' ';
    char promotionPiece = ' ';
    char turn = 'W';

    int legalMoveCount = 0;
    int pgnTurnCount = 1;
    int currentPosition[2] = {-1, -1};
    int desiredPosition[2] = {-1, -1};

    const string ERROR_AMBIGUOUS_MOVE = "This move is ambiguous!\n";
    const string ERROR_CANNOT_CASTLE_WHILE_IN_CHECK = "You cannot castle while in check!\n";
    const string ERROR_CANNOT_PROMOTE_PIECE = "You cannot promote a piece!\n";
    const string ERROR_ILLEGAL_MOVE = "This is not a legal move!\n";
    const string ERROR_INVALID_INPUT = "This input is invalid!\n";
    const string ERROR_INVALID_PGN_FILE = "The PGN file is invalid and could not be loaded!\n";
    const string ERROR_MUST_SPECIFY_PROMOTION_PIECE = "You must specify a promotion piece!\n";
    const string ERROR_OWN_KING_STILL_IN_CHECK = "Your king is still in check!\n";
    const string ERROR_PUTS_OWN_KING_IN_CHECK = "This puts your own king in check!\n";

    string playerInput = "";

    vector<string> loadedPgn;
    vector<string> pgn;

    Piece* selectedPiece;
    Piece* destinationPiece;
    Piece* board[8][8];

    initialiseBoard(board);

    // Loop to handle each player move until the game is finished
    while (1) {
        clearConsole();
        printBoard(turn, board);

        if (failedLoad) {
            failedLoad = false;

            cout << ERROR_INVALID_PGN_FILE;
        }

        check = kingInCheck(turn, board);

        if (check)
            checkmate = !hasLegalMove(turn, board);
        else
            stalemate = !hasLegalMove(turn, board);

        if (checkmate || stalemate)
            break;

        if (check) {
            turn == 'W' ? cout << "White's " : cout << "Black's ";
            cout << "king is in check!\n";
        }

        // Adds turn counter to PGN
        if (turn == 'W') {
            pgn.push_back(to_string(pgnTurnCount) + ".");
            pgnTurnCount++;
        }

        // Loop to handle player inputting their desired move
        while (legalMoveCount != 1) {
            if (loadingGame) {
                playerInput = loadedPgn.front();
            } else {
                turn == 'W' ? cout << "White, " : cout << "Black, ";
                cout << "please input your move:\n";
                cin >> playerInput;
            }

            // Checks if input is valid
            validInput = processPlayerInput(turn, &desiredPieceToMove, &promotionPiece, currentPosition, desiredPosition, playerInput, board);

            if (!validInput) {
                if (loadingGame) {
                    failedLoad = true;

                    break;
                }

                cout << ERROR_INVALID_INPUT;
                continue;
            }

            if (!playerInput.compare("draw")) {
                if (loadingGame) {
                    failedLoad = true;

                    break;
                }

                turn == 'W' ? cout << "Black, " : cout << "White, ";
                cout << "do you accept a draw? (Y/N)\n";
                cin >> playerInput;

                if (playerInput.compare("Y")) {
                    cout << "Draw declined\n";
                    continue;
                }

                drawAgreed = true;
                break;
            }

            if (!playerInput.compare("load")) {
                if (loadingGame) {
                    failedLoad = true;

                    break;
                }

                cout << "This will close your current game without saving! Would you like to continue? (Y/N)\n";
                cin >> playerInput;

                if (playerInput.compare("Y"))
                    continue;

                initialiseBoard(board);

                turn = 'W';

                loadingGame = true;

                loadedPgn = loadGame();

                continue;
            }

            if (!playerInput.compare("resign")) {
                if (loadingGame) {
                    failedLoad = true;

                    break;
                }

                resign = true;
                break;
            }

            if (!playerInput.compare("save")) {
                if (loadingGame) {
                    failedLoad = true;

                    break;
                }

                saveGame(pgn);
                continue;
            }

            // Handles castling
            if (playerInput.find("O-O") != string::npos) {
                if (check) {
                    if (loadingGame) {
                        failedLoad = true;

                        break;
                    }

                    cout << ERROR_CANNOT_CASTLE_WHILE_IN_CHECK;
                    continue;
                }

                if (!castlingValid(turn, currentPosition, desiredPosition, board)) {
                    if (loadingGame) {
                        failedLoad = true;

                        break;
                    }

                    continue;
                }

                legalMoveCount = 1;
            }

            // Prevents promoting for non-pawns
            if (desiredPieceToMove != 'P' && promotionPiece != ' ') {
                if (loadingGame) {
                    failedLoad = true;

                    break;
                }

                cout << ERROR_CANNOT_PROMOTE_PIECE;
                continue;
            }

            // Ensures promotion piece is specified if pawn reaches last rank
            if (desiredPieceToMove == 'P' && promotionPiece == ' ')
                if ((turn == 'W' && desiredPosition[0] == 7) || (turn == 'B' && desiredPosition[0] == 0)) {
                    if (loadingGame) {
                        failedLoad = true;

                        break;
                    }

                    cout << ERROR_MUST_SPECIFY_PROMOTION_PIECE;
                    continue;
                }

            // Ensures move is legal and unambiguous
            if (playerInput.find("O-O") == string::npos)  // skip if castling (handled above)
                legalMoveCount = checkingLegalMove(desiredPieceToMove, turn, currentPosition, desiredPosition, board);

            // Ensures en passant move is valid
            if (desiredPieceToMove == 'P' && currentPosition[1] != desiredPosition[1] && board[desiredPosition[0]][desiredPosition[1]]->getColour() == 'N')
                enPassant = true;
            else
                enPassant = false;

            if (legalMoveCount == 0) {
                if (loadingGame) {
                    failedLoad = true;

                    break;
                }

                cout << ERROR_ILLEGAL_MOVE;
                continue;
            }

            if (legalMoveCount > 1) {
                if (loadingGame) {
                    failedLoad = true;

                    break;
                }

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
                    board[desiredPosition[0]][desiredPosition[1]] = new PieceQueen(turn);
                    break;

                case 'R':
                    board[desiredPosition[0]][desiredPosition[1]] = new PieceRook(turn);
                    break;

                case 'B':
                    board[desiredPosition[0]][desiredPosition[1]] = new PieceBishop(turn);
                    break;

                case 'N':
                    board[desiredPosition[0]][desiredPosition[1]] = new PieceKnight(turn);
                    break;

                default:
                    board[desiredPosition[0]][desiredPosition[1]] = selectedPiece;
                    break;
            }

            promotionPiece = ' ';

            // Checks if move has put own king in check
            putSelfInCheck = kingInCheck(turn, board);

            if (putSelfInCheck) {
                if (loadingGame) {
                    failedLoad = true;

                    break;
                }

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

        // Resets the game if load failed
        if (failedLoad) {
            loadingGame = false;

            turn = 'W';

            loadedPgn.clear();

            initialiseBoard(board);

            continue;
        }

        if (resign || drawAgreed)
            break;

        legalMoveCount = 0;

        pgn.push_back(playerInput);

        turn == 'W' ? turn = 'B' : turn = 'W';

        // Sets pawns can't be captured by en passant if they pushed forward 2 squares last turn
        for (int rank = 0; rank < 8; rank++) {
            for (int file = 0; file < 8; file++) {
                if (board[rank][file]->getColour() != turn)
                    continue;

                board[rank][file]->setCanEnPassant(false);
            }
        }

        if (loadingGame) {
            if (loadedPgn.size() > 0)
                loadedPgn.erase(loadedPgn.begin());

            if (loadedPgn.size() == 0)
                loadingGame = false;
        }
    }

    if (checkmate || resign) {
        turn == 'W' ? pgn.push_back("0-1") : pgn.push_back("1-0");

        turn == 'W' ? cout << "Black " : cout << "White ";
        cout << "wins!\n";
    }

    if (drawAgreed || stalemate) {
        pgn.push_back("1/2-1/2");

        if (stalemate)
            cout << "Stalemate!\n";
        else
            cout << "Draw!\n";
    }

    cout << "Would you like to save this game? (Y/N)\n";
    cin >> playerInput;

    if (!playerInput.compare("Y"))
        saveGame(pgn);

    return 0;
}