#include "misc.hpp"

// Takes the player's input and interprets the intended move
bool processPlayerInput(char turn, char* pDesiredPieceToMove, char* pPromotedPiece, int currentPosition[2], int desiredPosition[2], string playerInput, Piece* board[8][8]) {
    string pieces = "KQRBN";
    string promotionPieces = "QRBN";
    string commands[] = {"draw", "load", "resign", "save"};

    if (find(begin(commands), end(commands), playerInput) != end(commands))
        return true;

    if (playerInput.back() == '+' || playerInput.back() == '#')
        playerInput.pop_back();

    // Handles castling input
    if (!playerInput.compare("O-O") || !playerInput.compare("O-O-O")) {
        if (turn == 'W') {
            if (board[0][4]->getName() != 'K' || board[0][4]->getHasMoved())
                return false;

            if (!playerInput.compare("O-O") && (board[0][7]->getName() != 'R' || board[0][7]->getHasMoved()))
                return false;

            if (!playerInput.compare("O-O-O") && (board[0][0]->getName() != 'R' || board[0][0]->getHasMoved()))
                return false;

            currentPosition[0] = 0;

            desiredPosition[0] = 0;
        }

        if (turn == 'B') {
            if (board[7][4]->getName() != 'K' || board[7][4]->getHasMoved())
                return false;

            if (!playerInput.compare("O-O") && (board[7][7]->getName() != 'R' || board[7][7]->getHasMoved()))
                return false;

            if (!playerInput.compare("O-O-O") && (board[7][0]->getName() != 'R' || board[7][0]->getHasMoved()))
                return false;

            currentPosition[0] = 7;

            desiredPosition[0] = 7;
        }

        *pDesiredPieceToMove = 'K';

        currentPosition[1] = 4;

        if (!playerInput.compare("O-O"))
            desiredPosition[1] = 6;

        if (!playerInput.compare("O-O-O"))
            desiredPosition[1] = 2;

        return true;
    }

    // Handles if pawn promotion is stated in desired move
    if (playerInput[playerInput.length() - 2] == '=') {
        if (find(begin(promotionPieces), end(promotionPieces), playerInput.back()) == end(promotionPieces))
            return false;

        *pPromotedPiece = playerInput.back();

        playerInput.pop_back();
        playerInput.pop_back();
    } else
        *pPromotedPiece = ' ';

    // Takes last element of playerInput to get desired rank
    if (charToInt(playerInput.back()) < 1 || charToInt(playerInput.back()) > 8)
        return false;
    else {
        desiredPosition[0] = charToInt(playerInput.back()) - 1;
        playerInput.pop_back();
    }

    // Takes last element of playerInput to get desired file
    if (letterToInt(playerInput.back()) < 0 || letterToInt(playerInput.back()) > 7)
        return false;
    else {
        desiredPosition[1] = letterToInt(playerInput.back());
        playerInput.pop_back();
    }

    if (playerInput.back() == 'x')
        playerInput.pop_back();

    // Stores piece's current rank if specified in input
    if (charToInt(playerInput.back()) > 0 && charToInt(playerInput.back()) < 9) {
        currentPosition[0] = charToInt(playerInput.back()) - 1;
        playerInput.pop_back();
    } else
        currentPosition[0] = -1;

    // Stores piece's current file if specified in input
    if (letterToInt(playerInput.back()) >= 0 && letterToInt(playerInput.back()) < 8) {
        currentPosition[1] = letterToInt(playerInput.back());
        playerInput.pop_back();
    } else
        currentPosition[1] = -1;

    // Checks if the input supplies a pawn move or a piece move
    if (playerInput.empty()) {
        *pDesiredPieceToMove = 'P';
    } else if (find(begin(pieces), end(pieces), playerInput[0]) != end(pieces)) {
        *pDesiredPieceToMove = playerInput[0];
    } else
        return false;

    return true;
}

// Converts an integer stored as a char to an int
// When casting from char to int, the "- '0'" portion is required to convert the integer's ASCII code to the actual integer
int charToInt(char character) {
    return (int)character - '0';
}

// Converts a lowercase letter stored as a char to an int, where a = 0, b = 1, etc...
int letterToInt(char character) {
    return (int)character - 'a';
}

// Saves current game to PGN.txt file
void saveGame(vector<string> moves) {
    ofstream outputFile("./PGN.txt");

    ostream_iterator<string> outputIterator(outputFile, " ");
    copy(begin(moves), end(moves), outputIterator);

    outputFile.close();
}

// Clears console
// Taken from https://stackoverflow.com/questions/6486289/how-can-i-clear-console
void clearConsole() {
#if defined _WIN32
    system("cls");
    // clrscr(); // including header file : conio.h
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
    // std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences
#elif defined(__APPLE__)
    system("clear");
#endif
}

// Loads game from PGN.txt file
vector<string> loadGame() {
    string pgnElement = " ";

    vector<string> loadedPgn;

    ifstream inputFile("./PGN.txt");

    while (!inputFile.eof()) {
        inputFile >> pgnElement;

        if (pgnElement.back() == '.')
            continue;

        loadedPgn.push_back(pgnElement);
    }

    inputFile.close();

    return loadedPgn;
}
