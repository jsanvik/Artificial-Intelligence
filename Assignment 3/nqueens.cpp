// Using the CSPs, implement the N-queen problem, with the local search and iterative improvement algorithm.

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <array>
#include <limits>
#include <cmath>
#include <cstdlib>

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
    
    // We only move queens in their own column, since there should only be one queen per column anyway
    /**
     * @param x: Column of queen to be moved (will not change)
     * @param y: New row to place queen in
     * @return returns the old y coordinate we move the queen from (useful if we need to move it back)
     */
    int moveQueen(int x, int y) {
        if (x > size || y > size) {std::cout << "TOO BIG";}
        // Find current y coordinate of queen in column x
        int oldy = queens[x][1];
        std::cout << "OLDY " << oldy << std::endl;
        squares[oldy][x]->removeQueen();
        squares[y][x]->placeQueen();
        queens[x] = {x, y};
        return oldy;
    }

    // Count attacks in one row
    int rowattacks(int x, int y) {
        int rowqueens = 0; // Number of queens in current row
        int rowattacks = 0; // Number of attacks in current row
        for (int i = 0; i < size; ++i) {
            if (squares[y][i]->isQueen()) {rowqueens++;}
        }
        if (rowqueens > 1) {rowattacks+=(rowqueens-1);} // Count one attack in a row per pair of queens
        return rowattacks;
    }

    // Count attacks in one column
    int columnattacks(int x, int y) {
        int columnqueens = 0; // Number of queens in current row
        int columnattacks = 0; // Number of attacks in current row
        for (int i = 0; i < size; ++i) {
            if (squares[i][x]->isQueen()) {columnqueens++;}
        }
        if (columnqueens > 1) {columnattacks+=(columnqueens-1);} // Count one attack in a column per pair of queens
        return columnattacks;
    }
    // int diagonalattacks(int x, int y) {
    //     int diagonalqueens = 0;
    //     int diagonalattacks = 0;
    //     // Check down and right
    //     int i = x;
    //     int j = y;
    //     while(i < size && j < size) {
    //         if (squares[j][i]->isQueen()) {diagonalqueens++;}
    //         i++; j++;
    //     }
    //     // Check down and left
    //     i = x-1;
    //     j = y+1;
    //     while(i >= 0 && j < size) {
    //         if (squares[j][i]->isQueen()) {diagonalqueens++;}
    //         i--; j++;
    //     }
    //     // Check up and left
    //     i = x-1;
    //     j = y-1;
    //     while(i >= 0 && j >= 0) {
    //         if (squares[j][i]->isQueen()) {diagonalqueens++;}
    //         i--; j--;
    //     }
    //     // Check down and left
    //     i = x+1;
    //     j = y-1;
    //     while(i < size && j >= 0) {
    //         if (squares[j][i]->isQueen()) {diagonalqueens++;}
    //         i++; j--;
    //     }
    //     if (diagonalqueens > 1) {diagonalattacks+=(diagonalqueens-1);}
    //     return diagonalattacks;
    // }

    int diagonalattacks(int queen1, int queen2) {
        int diagonalattacks = 0;
        if (abs(queens[queen1][0] - queens[queen2][0]) == abs(queens[queen1][1] - queens[queen2][1])) {diagonalattacks++;}
        return diagonalattacks;
    }

    // count attacks for entire board
    int count_attacks() {
        int r = 0; // Row attacks
        int c = 0; // Column attacks
        int d = 0; // Diagonal attacks
        for(int i = 0; i < size; ++i) {
            r += rowattacks(i, i);
            c += columnattacks(i, i);
        }
        for (int i = 0; i < queens.size(); ++i) {
            for (int j = 1; j < queens.size(); ++j) {
                if (i == j) {continue;}
                d += diagonalattacks(i, j);
            }
        }
        return r + c + d;
    }
    // count attacks for single square
    int count_attacks(int x, int y) {
        int r = rowattacks(x, y);
        int c = columnattacks(x, y);
        int d = diagonalattacks(x, y);
        return r + c + d;
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
    srand(time(0));
    int n;
    n = 8;
    // std::cout << "ENTER BOARD SIZE: ";
    // std::cin >> n;
    // std::cout << std::endl; 

    Board board(n);
    board.print();

    while(board.count_attacks() > 0) {
        int attacks = board.count_attacks();
        // RANDOMLY PICK A QUEEN THAT HAS CONFLICTS
        int queen = rand() % n;
        // RANDOMLY MOVE IT TO A SQUARE
        int square = rand() % n;
        int oldsquare = board.moveQueen(queen, square);
        // // If the new position is worse, move the queen back
        // if (attacks >= board.count_attacks()) {board.moveQueen(queen, oldsquare);}
    }

    board.print();

    std::cout << "ATTACKS: " << board.count_attacks() << std::endl;
    std::cout << "END OF PROGRAM \n";
    return 0;
}