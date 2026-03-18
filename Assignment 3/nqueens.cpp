#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <array>
#include <limits>
#include <cmath>

class Square {
  private:
  int x;
  int y;  
  bool queen;
  public:
    Square(int x, int y) {
        x = x; y = y;
        queen = false;
    }
    bool isQueen() {return queen;}
    bool placeQueen() {queen = true; return isQueen();}
    bool removeQueen() {queen = false; return isQueen();}
    int getx() {return x;} 
    int gety() {return y;}
};

class Board {
  private:
    int size;
    std::vector<std::vector<Square*>> squares;
    std::vector<std::array<int, 2>> queens; // Coordinates of all the queens
  public:
    // constructor
    Board(int n) {
        size = n;
        for (int i = 0; i < n; ++i) {
            std::vector<Square*> v;
            for (int j = 0; j < n; ++j) {
                Square* square = new Square(j, i);
                v.push_back(square);
            }
            v[0]->placeQueen(); // For simplicity's sake, initial setup will have one queen on each square in first row
            std::array<int, 2> coordinates = {0, i};
            queens.push_back(coordinates);
            squares.push_back(v);
        }
    };
    
    // Checks if x1y1 contains queen and x2y2 is empty; if so moves queen and returns true, else false
    bool moveQueen(int x1, int y1, int x2, int y2) { // Reassign queen
        if ((squares[y1][x1]->isQueen()) && !(squares[y2][x2]->isQueen())) {
            squares[y1][x1]->removeQueen();
            squares[y2][x2]->placeQueen();
            return true;
        } else {
            return false;
        }
    }

    // Goal test: Is there exactly one queen in each row, column, and not more than one in each diagonal?
    bool goalTest() {
        // Count queens horizontally and vertically (one in each row/column) and count total queens (should equal 8)
        int totalqueens = 0; // Total number of queens on the board
        for (int i = 0; i < size; ++i) {
            int rowqueens = 0; // Number of queens in current row
            int columnqueens = 0; // Number of queens in current column
            for (int j = 0; j < size; ++j) {
                if (squares[i][j]->isQueen()) {rowqueens++;}
                if (squares[j][i]->isQueen()) {columnqueens++;}
            }
            if (rowqueens != 1) {return false;}
            if (columnqueens != 1) {return false;}
        }
        if (totalqueens != 8) {return false;}

        return true;
    }

    // print the board
    void print() {
        std::cout << " ABCDEFGH\n"; // TODO: Fix this with other board sizes than 8
        for (int i = size-1; i >=0; --i) {
            std::cout << i+1;
            for (int j = 0; j < size; ++j) {
                if (squares[j][i]->isQueen()) {std::cout << "Q";}
                else {std::cout << "-";}
            }
            std::cout << size-i << std::endl;
        }
        std::cout << " ABCDEFGH\n\n"; // TODO: Fix this with other board sizes than 8
    }
};

int main() {
    int n;
    n = 8;
    // std::cout << "ENTER BOARD SIZE: ";
    // std::cin >> n;
    // std::cout << std::endl; 

    Board board(n);
    board.print();
    std::cout << board.goalTest() << std::endl;
    std::cout << "END OF PROGRAM \n";
    return 0;
}