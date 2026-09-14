#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum Difficulty {EASY, HARD};

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
            if (grid[i][0] == symbol && grid[i][1] == symbol && grid[i][2] == symbol) { // rows
                return true;
            }
            if (grid[0][i] == symbol && grid[1][i] == symbol && grid[2][i] == symbol) { // columns
                return true;
            }
        }

        
        if (grid[0][0] == symbol && grid[1][1] == symbol && grid[2][2] == symbol) { // diagonal 1
            return true;
        }
        if (grid[0][2] == symbol && grid[1][1] == symbol && grid[2][0] == symbol) { // diagonal 2
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


};
int main(){

 return 0;
}
