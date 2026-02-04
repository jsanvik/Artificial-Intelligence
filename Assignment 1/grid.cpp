/**
* @author John S. Anvik (j.anvik@uleth.ca)
* @date 2026-02-04
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

// CHaracters to represent grid squares
bool entering_forbidden_squares = true;
const std::string EMPTY_SQUARE = "-";
const std::string STARTING_SQUARE = "S";
const std::string GOAL_SQUARE = "G";
const std::string FORBIDDEN_SQUARE = "X";

class Agent;
void print_grid(std::vector<std::string> g);
bool valid_square(int x, int y, std::vector<std::string> g);

int main() {
    int x, y;
    std::vector<std::string> grid = {};
    bool goal = false;
    
    // prompt the user to enter the width and the height of the grid, the start and the goal states, and the forbidden squares.
    
    // Initialize Grid
    // A vertical vector of horizontal strings
    // Grid spaces are 1-indexed (vector row 0 and string index 0 print grid index numbers)
    std::cout << "\n Enter grid width and height in the following form: x y ";
    std::cin >> x >> y;
    for (int i = 0; i <= y; ++i) {
        std::string v = std::to_string(i);
        for (int j = 0; j < x; ++j) {
            if (i == 0) {v.append(std::to_string(j+1));}
            else {v.append(EMPTY_SQUARE);}
        }
        grid.push_back(v);
    }
    print_grid(grid);
    x = 0; y = 0; // Reset x and y to invalid coordinates
    
    // Add starting square in grid
    while (!valid_square(x, y, grid)) {
        std::cout << "\n Enter starting coordinates in the following form: x y ";
        std::cin >> x >> y; 
        // Check for valid starting square coordinates, then place starting square
        if (valid_square(x, y, grid)) {
            grid[y].replace(x, 1, STARTING_SQUARE);
        } else {std::cout << "INVALID STARTING SQUARE\n";}
    }
    print_grid(grid);
    x = 0; y = 0; // Reset x and y to invalid coordinates

    while (!valid_square(x, y, grid)) {
        std::cout << "\n Enter goal coordinates in the following form: x y ";
        std::cin >> x >> y; 
        // Check for valid goal coordinates, then place goal square
        if (valid_square(x, y, grid)) {
            grid[y].replace(x, 1, GOAL_SQUARE);
        } else {std::cout << "INVALID GOAL SQUARE\n";}
    }
    print_grid(grid);
    x = 0; y = 0; // Reset x and y to invalid coordinates
    
    // Enter forbidden squares
    while (entering_forbidden_squares) {
        std::cout << "\n To place a forbidden square, enter coordinates in the following form: x y \n or enter -1 -1 to place no more forbidden squares \n";
        std::cin >> x >> y;
        if (x !=-1 && y!=-1) {
            if (valid_square(x, y, grid)) {
                grid[y].replace(x, 1, "X");
                print_grid(grid);
            } else {
                std::cout << "INVALID SQUARE" << std::endl;
            }
        } else {entering_forbidden_squares = false;}
    }

    x = 0; y = 0; // Reset x and y to invalid coordinates

    while(!goal) {
        
    }

    return 0;
}

/**
 * @return true if square is valid to move to, otherwise false
 * @param g grid (vertical vector of horizontal strings)
 */
bool valid_square(int x, int y, std::vector<std::string> g) {
    return (x > 0 && y > 0 && y < g.size() && x < g.at(0).length() && g[y].substr(x, 1) != FORBIDDEN_SQUARE);
}

/**
 * @function 
 * @param x x coordinate
 * @param y y coordinate
 * @param g grid (vertical vector of horizontal strings)
*/
void print_grid(std::vector<std::string> g) {
    std::cout << std::endl;
    for (std::string s : g) {std::cout << s << std::endl;}
}

class Agent {
    private: 
    int x, y;
    std::vector<std::string>* grid;
    public:
    /**
     * @brief default constructor
     */
    Agent(int startx, int starty, std::vector<std::string> g) {
        x = startx; y = starty;
        grid = &g;
    }
    /**
     * @brief move agent up
     * @return if valid move, moves agent and returns true, else returns false
     */
    bool up() {
        if (valid_square(x, y+1, *grid)) {
            y+=1;
            return true;
        } else {return false;}
    }
    /**
     * @brief move agent left
     * @return if valid move, moves agent and returns true, else returns false
     */
    bool left() {
        if (valid_square(x-1, y, *grid)) {
            x-=1;
            return true;
        } else {return false;}
    }
    /**
     * @brief move agent right
     * @return if valid move, moves agent and returns true, else returns false
     */
    bool right() {
        if (valid_square(x+1, y, *grid)) {
            x+=1;
            return true;
        } else {return false;}
    }
    /**
     * @brief move agent down
     * @return if valid move, moves agent and returns true, else returns false
     */
    bool down() {
        if (valid_square(x, y-1, *grid)) {
            y-=1;
            return true;
        } else {return false;}
    }
    bool checkGoalState(int currentx, int currenty, std::vector<std::string> g) {
        return(x > 0 && y > 0 && y < g.size() && x < g.at(0).length() && g[y].substr(x, 1) == GOAL_SQUARE);
    }
};