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
