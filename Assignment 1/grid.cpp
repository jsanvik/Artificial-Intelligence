/**
* @author John S. Anvik (j.anvik@uleth.ca)
* @date 2026-01-30
*/
/* Given a grid similar to the one in Figure 1. You may prompt the user to: enter: 
- the width and the height of the grid, 
- the start and the goal states, and 
- the forbidden squares.  
Implement each of the following searches and show the solutions:  
(a) A depth-first search from S to G, given that the order of the operators you will test
is: up, left, right, then down.  
(b) A breadth-first search from S to G, given that the order of the operators you will test
is: up, left, right, then down.  
(c) An iterative deepening depth-first search from S to G, given that the order of the
operators you will test is: up, left, right, then down. At what depth the solution is
reached?  */

#include <iostream>
#include <vector>
#include <string>

int main() {
    int x, y;
    std::vector<std::vector<char>> grid;
    bool goal = false;
    bool entering_forbidden_squares = true;
    
    // prompt the user to enter the width and the height of the grid, the start and the goal states, and the forbidden squares.
    std::cout << "\n Enter grid width and height in the following form: x y ";
    std::cin >> x >> y;
    for (int i = x; i < x; ++i) {
        std::vector<char> v;
        for (int j = y; j < y; ++j) {
            v.push_back('O');
        }
        grid.push_back(v);
    }
    std::cout << "\n Enter starting coordinates in the following form: x y ";
    std::cin >> x >> y; 
    grid[x-1][y-1] = 'S';
    std::cout << "\n Enter goal coordinates in the following form: x y ";
    std::cin >> x >> y; 
    grid[x-1][y-1] = 'G';
    while (entering_forbidden_squares) {
        std::cout << "\n To place a forbidden square, enter coordinates in the following form: x y \n or enter -1 to place no more forbidden squares \n";
        std::cin >> x >> y;
        if (x !=-1 && y!=-1) {
            if (grid[x-1][y-1] != 'S' && grid[x-1][y-1] != 'G') {
                grid[x-1][y-1] = 'X';
            } else {
                std::cout << "INVALID SQUARE: " << grid[x-1][y-1] << std::endl;
            }
        } else {entering_forbidden_squares = false;}
    }


    while(!goal) {

    }

    return 0;
}