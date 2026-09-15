#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


using namespace std;

enum Difficulty {EASY, HARD};



class Board{
private:
    vector<vector<char>> grid;
    int Size;

public:
    Board(int inputSize = 3){
        Size = inputSize;
        grid.resize(Size);
        for(int i =0; i < Size; i++)
         grid[i].resize(Size, ' ');
    }



    bool checkWin(char symbol) const {
        for (int i = 0; i < Size; i++) {
            if (grid[i][0] == symbol && grid[i][1] == symbol && grid[i][2] == symbol) { //rows
                return true;
            }
            if (grid[0][i] == symbol && grid[1][i] == symbol && grid[2][i] == symbol) { //coulms
                return true;
            }
        }

        if (grid[0][0] == symbol && grid[1][1] == symbol && grid[2][2] == symbol) { //digonal 1
            return true;
        }
        if (grid[0][2] == symbol && grid[1][1] == symbol && grid[2][0] == symbol) { //digonal 2
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
        if(row >= 0 && row < Size
        && col >= 0 && col < Size)
        {
            return grid[row][col];
        }
        return ' ';
    }

    void reset(){
        for(int i = 0; i < Size; i++){
            for(int j = 0; j < Size; j++){
                grid[i][j] = ' ';
            }
        }
    }

    bool makeMove(int row, int col, char symbol)
    {
        if (!isValidMove(row, col))
        {
            return false;
        }
        else
        {
            grid[row][col] = symbol;
            return true;
        }
    }

    // Needed by minimax to backtrack a hypothetical move.
    void undoMove(int row, int col)
    {
        if (row >= 0 && row < Size && col >= 0 && col < Size)
        {
            grid[row][col] = ' ';
        }
    }

    bool isValidMove(int row, int col) const
    {
        if (row >= Size || row < 0 || col >= Size || col < 0)
        {
            return false;
        }
     
        if (grid[row][col] != ' ')
        {
            return false;
        }
         
        return true;
    }
    void display()
    {
        for (int i = 1; i <= Size; i++)
        {
            if (i == 1)
            {
                cout << "    " << i;
            }
            else
            {
                cout << "   " << i;
            }
        }
        cout << endl;
        for (int i = 0; i < Size; i++)
        {
            cout << i+1 <<" | ";
            for (int j = 0; j < Size; j++)
            {
                cout << grid[i][j] << " | ";
            }
            cout << endl;
        }
    }

};



class Player{
protected:
  string name;
  char symbol;
public:
  Player(string inputName, char inputSymbol)
  {
    name = inputName;
    symbol = inputSymbol;
  }

  virtual ~Player()
  {}

  virtual void getMove(int& row, int& col)= 0;

  string getName() const
  {
    return name;
  }
  char getSymbol() const
  {
    return symbol;
  }

  void setName(string Name)
  {
    name = Name;
  }
};



class AIPlayer:public Player{
  private:

   Difficulty difficulty;
   Board* board; // pointer to the board this AI is currently playing on

   char getOpponentSymbol() const {
     return (symbol == 'X') ? 'O' : 'X';
   }

   // Recursive minimax scorer.
   // isMaximizing = true  -> it's the AI's turn to move in this hypothetical
   // isMaximizing = false -> it's the human's turn to move in this hypothetical
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
     } else {
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
   AIPlayer(const string& n, char s, Difficulty d):Player(n,s),difficulty(d),board(nullptr){}

   void setDifficulty(Difficulty newDifficulty){
     difficulty=newDifficulty;

   }

   // Lets the game loop tell the AI which board it's playing on,
   // since the inherited getMove(row, col) signature has no board parameter.
   void setBoard(Board& b) {
     board = &b;
   }

   void getRandomMove(const Board& board, int& row, int& col)const {
     vector<pair<int,int>> valid;
     int size=board.getSize();
     for(int i=0;i<size;i++){
      for(int j=0;j<size;j++){

        bool isValid=board.isValidMove(i,j);

        if(isValid){
          valid.push_back({i,j});
        }

      }
     }
     if(valid.empty()){
       return;
     }
     int r=rand()%valid.size();
     row=valid[r].first;
     col=valid[r].second;

   }

   // +10 if the AI has won this hypothetical board, -10 if the human has, else 0.
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
     } else {
       getBestMove(*board, row, col);
     }
   }

};



int main(){

 return 0;
}
