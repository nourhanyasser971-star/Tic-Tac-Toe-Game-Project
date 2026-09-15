#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

enum Difficulty { EASY, HARD };



// =====================================================
// ====================== BOARD =========================
// =====================================================

class Board {
private:
    vector<vector<char>> grid;
    int Size;

public:
    Board(int inputSize = 3) {
        Size = inputSize;
        grid.resize(Size);

        for (int i = 0; i < Size; i++)
            grid[i].resize(Size, ' ');
    }


    bool checkWin(char symbol) const {
        for (int i = 0; i < Size; i++) {

            // Rows
            if (grid[i][0] == symbol &&
                grid[i][1] == symbol &&
                grid[i][2] == symbol) {
                return true;
            }

            // Columns
            if (grid[0][i] == symbol &&
                grid[1][i] == symbol &&
                grid[2][i] == symbol) {
                return true;
            }
        }

        // Diagonal 1
        if (grid[0][0] == symbol &&
            grid[1][1] == symbol &&
            grid[2][2] == symbol) {
            return true;
        }

        // Diagonal 2
        if (grid[0][2] == symbol &&
            grid[1][1] == symbol &&
            grid[2][0] == symbol) {
            return true;
        }

        return false;
    }


    bool isFull() const {
        for (int i = 0; i < Size; i++) {
            for (int j = 0; j < Size; j++) {
                if (grid[i][j] == ' ') {
                    return false;
                }
            }
        }

        return true;
    }


    int getSize() const {
        return Size;
    }


    char getCell(int row, int col) const {
        if (row >= 0 && row < Size &&
            col >= 0 && col < Size) {
            return grid[row][col];
        }

        return ' ';
    }


    void reset() {
        for (int i = 0; i < Size; i++) {
            for (int j = 0; j < Size; j++) {
                grid[i][j] = ' ';
            }
        }
    }


    bool makeMove(int row, int col, char symbol) {
        if (!isValidMove(row, col)) {
            return false;
        }
        else {
            grid[row][col] = symbol;
            return true;
        }
    }


    // Needed by minimax to backtrack a hypothetical move.
    void undoMove(int row, int col) {
        if (row >= 0 && row < Size &&
            col >= 0 && col < Size) {
            grid[row][col] = ' ';
        }
    }


    bool isValidMove(int row, int col) const {
        if (row >= Size || row < 0 ||
            col >= Size || col < 0) {
            return false;
        }

        if (grid[row][col] != ' ') {
            return false;
        }

        return true;
    }


    void display() {
        for (int i = 1; i <= Size; i++) {
            if (i == 1) {
                cout << "    " << i;
            }
            else {
                cout << "   " << i;
            }
        }

        cout << endl;

        for (int i = 0; i < Size; i++) {
            cout << i + 1 << " | ";

            for (int j = 0; j < Size; j++) {
                cout << grid[i][j] << " | ";
            }

            cout << endl;
        }
    }
};



// =====================================================
// ====================== PLAYER ========================
// =====================================================

class Player {
protected:
    string name;
    char symbol;

public:
    Player(string inputName, char inputSymbol) {
        name = inputName;
        symbol = inputSymbol;
    }

    virtual ~Player() {}

    virtual void getMove(int& row, int& col) = 0;

    string getName() const {
        return name;
    }

    char getSymbol() const {
        return symbol;
    }

    void setName(string Name) {
        name = Name;
    }
};



// =====================================================
// ================== HUMAN PLAYER ======================
// =====================================================

class HumanPlayer : public Player {
public:
    HumanPlayer(string n, char s) : Player(n, s) {}

    void getMove(int& row, int& col) override {
        cout << "Enter row and column: ";
        cin >> row >> col;

        row--;
        col--;
    }
};



// =====================================================
// ====================== AI PLAYER =====================
// =====================================================

class AIPlayer : public Player {
private:

    Difficulty difficulty;
    Board* board;


    char getOpponentSymbol() const {
        return (symbol == 'X') ? 'O' : 'X';
    }


    // Recursive minimax scorer.
    int minimax(Board& b, bool isMaximizing) const {

        if (b.checkWin(symbol)) {
            return 10;
        }

        if (b.checkWin(getOpponentSymbol())) {
            return -10;
        }

        if (b.isFull()) {
            return 0;
        }

        int size = b.getSize();


        if (isMaximizing) {

            int bestScore = -1000;

            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {

                    if (b.isValidMove(i, j)) {

                        b.makeMove(i, j, symbol);

                        int score = minimax(b, false);

                        b.undoMove(i, j);

                        bestScore = max(bestScore, score);
                    }
                }
            }

            return bestScore;
        }


        else {

            int bestScore = 1000;

            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {

                    if (b.isValidMove(i, j)) {

                        b.makeMove(i, j, getOpponentSymbol());

                        int score = minimax(b, true);

                        b.undoMove(i, j);

                        bestScore = min(bestScore, score);
                    }
                }
            }

            return bestScore;
        }
    }


public:

    AIPlayer(const string& n, char s, Difficulty d)
        : Player(n, s), difficulty(d), board(nullptr) {}


    void setDifficulty(Difficulty newDifficulty) {
        difficulty = newDifficulty;
    }


    void setBoard(Board& b) {
        board = &b;
    }


    void getRandomMove(const Board& board, int& row, int& col) const {

        vector<pair<int, int>> valid;

        int size = board.getSize();

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {

                bool isValid = board.isValidMove(i, j);

                if (isValid) {
                    valid.push_back({ i, j });
                }
            }
        }


        if (valid.empty()) {
            return;
        }


        int r = rand() % valid.size();

        row = valid[r].first;
        col = valid[r].second;
    }


    int evaluateBoard(const Board& board) const {

        if (board.checkWin(symbol)) {
            return 10;
        }

        if (board.checkWin(getOpponentSymbol())) {
            return -10;
        }

        return 0;
    }


    void getBestMove(Board& board, int& row, int& col) const {

        int bestScore = -1000;

        row = -1;
        col = -1;

        int size = board.getSize();


        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {

                if (board.isValidMove(i, j)) {

                    board.makeMove(i, j, symbol);

                    int score = minimax(board, false);

                    board.undoMove(i, j);


                    if (score > bestScore) {

                        bestScore = score;

                        row = i;
                        col = j;
                    }
                }
            }
        }
    }


    void getMove(int& row, int& col) override {

        if (board == nullptr) {
            row = -1;
            col = -1;
            return;
        }


        if (difficulty == EASY) {
            getRandomMove(*board, row, col);
        }
        else {
            getBestMove(*board, row, col);
        }
    }
};



// =====================================================
// ====================== GAME ==========================
// =====================================================

class Game {
private:

    Board board;

    Player* player1;
    Player* player2;

    Player* currentPlayer;

    bool exitGame;


public:

    // Constructor
    Game() {

        player1 = nullptr;
        player2 = nullptr;
        currentPlayer = nullptr;

        exitGame = false;
    }


    // Destructor
    ~Game() {

        delete player1;
        delete player2;
    }



    // =================================================
    // ================== PART 6 ========================
    // =================================================


    // Display the main menu and select game mode
    void showMenu() {

        int choice;


        while (true) {

            cout << endl;
            cout << "==================================" << endl;
            cout << "          TIC TAC TOE" << endl;
            cout << "==================================" << endl;

            cout << "1. Player vs Player" << endl;
            cout << "2. Player vs Computer - Easy" << endl;
            cout << "3. Player vs Computer - Hard" << endl;
            cout << "4. Exit" << endl;

            cout << "Enter your choice: ";


            if (!(cin >> choice)) {

                cout << "Invalid input. Please enter a number." << endl;

                cin.clear();

                cin.ignore(
                    numeric_limits<streamsize>::max(),
                    '\n'
                );

                continue;
            }


            if (choice == 1) {

                setupPvP();

                return;
            }


            else if (choice == 2) {

                setupPvC(EASY);

                return;
            }


            else if (choice == 3) {

                setupPvC(HARD);

                return;
            }


            else if (choice == 4) {

                exitGame = true;

                return;
            }


            else {

                cout << "Invalid choice. Please choose 1, 2, 3, or 4." << endl;
            }
        }
    }



    // Setup Player vs Player
    void setupPvP() {

        string name1;
        string name2;


        // Clean previous players if replaying
        delete player1;
        delete player2;

        player1 = nullptr;
        player2 = nullptr;


        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );


        cout << endl;
        cout << "Enter Player 1 name: ";
        getline(cin, name1);


        cout << "Enter Player 2 name: ";
        getline(cin, name2);


        player1 = new HumanPlayer(name1, 'X');
        player2 = new HumanPlayer(name2, 'O');


        currentPlayer = player1;
    }



    // Setup Player vs Computer
    void setupPvC(Difficulty difficulty) {

        string name;


        // Clean previous players if replaying
        delete player1;
        delete player2;

        player1 = nullptr;
        player2 = nullptr;


        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );


        cout << endl;
        cout << "Enter your name: ";
        getline(cin, name);


        player1 = new HumanPlayer(name, 'X');

        player2 = new AIPlayer(
            "Computer",
            'O',
            difficulty
        );


        // Tell the AI which board it is playing on
        AIPlayer* aiPlayer =
            dynamic_cast<AIPlayer*>(player2);


        if (aiPlayer != nullptr) {
            aiPlayer->setBoard(board);
        }


        currentPlayer = player1;
    }



    // =================================================
    // ================== PART 7 ========================
    // =================================================


    // Handle a human player's turn
    void handleHumanMove(Player& player) {

        int row;
        int col;


        while (true) {

            cout << endl;
            cout << player.getName()
                 << " (" << player.getSymbol()
                 << "), it's your turn." << endl;


            cout << "Enter row and column (1-3): ";


            if (!(cin >> row >> col)) {

                cout << "Invalid input. Please enter numbers only."
                     << endl;

                cin.clear();

                cin.ignore(
                    numeric_limits<streamsize>::max(),
                    '\n'
                );

                continue;
            }


            // Convert from 1-3 to 0-2
            row--;
            col--;


            if (!board.isValidMove(row, col)) {

                cout << "Invalid move. "
                     << "The cell is either occupied or out of range."
                     << endl;

                continue;
            }


            board.makeMove(
                row,
                col,
                player.getSymbol()
            );


            break;
        }
    }



    // Handle AI player's turn
    void handleAIMove(AIPlayer& aiPlayer) {

        int row = -1;
        int col = -1;


        cout << endl;
        cout << aiPlayer.getName()
             << " (" << aiPlayer.getSymbol()
             << ") is thinking..." << endl;


        aiPlayer.getMove(row, col);


        if (row >= 0 && col >= 0) {

            board.makeMove(
                row,
                col,
                aiPlayer.getSymbol()
            );


            cout << "Computer played: "
                 << "(" << row + 1
                 << ", " << col + 1
                 << ")" << endl;
        }
    }



    // Switch between the two players
    void switchPlayer() {

        if (currentPlayer == player1) {
            currentPlayer = player2;
        }
        else {
            currentPlayer = player1;
        }
    }



    // Check whether the current game has ended
    bool checkGameEnd() {

        // Current player won
        if (board.checkWin(currentPlayer->getSymbol())) {
            return true;
        }


        // Board is full -> Draw
        if (board.isFull()) {
            return true;
        }


        return false;
    }



    // Display the result of the game
    void displayResult() {

        cout << endl;
        cout << "==================================" << endl;


        if (board.checkWin(currentPlayer->getSymbol())) {

            cout << "Winner: "
                 << currentPlayer->getName()
                 << " (" << currentPlayer->getSymbol()
                 << ")" << endl;
        }


        else if (board.isFull()) {

            cout << "The game is a DRAW!" << endl;
        }


        cout << "==================================" << endl;
    }



    // Start and control the complete game
    void start() {

        while (!exitGame) {

            // Show menu and setup the game
            showMenu();


            if (exitGame) {
                break;
            }


            // Make sure board is empty
            board.reset();


            bool gameEnded = false;


            // Game turns
            while (!gameEnded) {

                board.display();


                // Human player's turn
                HumanPlayer* human =
                    dynamic_cast<HumanPlayer*>(currentPlayer);


                if (human != nullptr) {

                    handleHumanMove(*human);
                }


                else {

                    // AI player's turn
                    AIPlayer* ai =
                        dynamic_cast<AIPlayer*>(currentPlayer);


                    if (ai != nullptr) {

                        handleAIMove(*ai);
                    }
                }


                // Check win or draw
                gameEnded = checkGameEnd();


                if (gameEnded) {

                    board.display();

                    displayResult();

                    break;
                }


                // Switch player
                switchPlayer();
            }


            // Ask for replay
            char replay;


            while (true) {

                cout << endl;
                cout << "Do you want to play again? (y/n): ";


                cin >> replay;


                if (replay == 'y' ||
                    replay == 'Y') {

                    break;
                }


                else if (replay == 'n' ||
                         replay == 'N') {

                    exitGame = true;

                    break;
                }


                else {

                    cout << "Invalid input. "
                         << "Please enter y or n."
                         << endl;
                }
            }
        }


        cout << endl;
        cout << "Thank you for playing!" << endl;
    }
};



// =====================================================
// ======================== MAIN ========================
// =====================================================

int main() {

    srand(static_cast<unsigned int>(time(nullptr)));


    Game game;

    game.start();


    return 0;
}
