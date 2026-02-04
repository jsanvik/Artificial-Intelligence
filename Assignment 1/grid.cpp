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
#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include <array>

// CHaracters to represent grid squares
bool entering_forbidden_squares = true;
const std::string EMPTY_SQUARE = "-";
const std::string STARTING_SQUARE = "S";
const std::string GOAL_SQUARE = "G";
const std::string FORBIDDEN_SQUARE = "X";

class Agent;

// Helper functions

/**
 * @param g grid (vertical vector of horizontal strings)
 */
void print_grid(std::vector<std::string> g);

/**
 * @param x x coordinate
 * @param y y coordinate
 * @param g grid (vertical vector of horizontal strings)
 * @return true if square is valid to move to, otherwise false
*/
bool valid_square(int x, int y, std::vector<std::string> g);

bool check_coordinate_history(int x, int y, std::vector<std::array<int, 2>> history);

bool dfs(std::vector<std::string> grid, std::stack<std::array<int, 2>> fringe, std::vector<std::array<int, 2>> path);

int main() {
    int x, y;
    std::vector<std::string> grid = {};
    std::stack<std::array<int, 2>> starting_stack; // Stack used for DFS fringe
    std::vector<std::array<int, 2>> history; // Vector used to store current path
    
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
            starting_stack.push({x, y});
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
                grid[y].replace(x, 1, FORBIDDEN_SQUARE);
                print_grid(grid);
            } else {
                std::cout << "INVALID SQUARE" << std::endl;
            }
        } else {entering_forbidden_squares = false;}
    }

    x = 0; y = 0; // Reset x and y to invalid coordinates


    dfs(grid, starting_stack, history);

    return 0;
}


bool valid_square(int x, int y, std::vector<std::string> g) {
    return (x > 0 && y > 0 && y < g.size() && x < g.at(0).length() && g[y].substr(x, 1) != FORBIDDEN_SQUARE);
}


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
    /**
     * @brief checks if agent has reached goal square
     * @return returns true if agent has reached goal square, otherwise returns false
     */
    // bool checkGoalState() {
    //     std::string s = *grid;
    //     return(s[y].substr(x, 1) == GOAL_SQUARE);
    // }
};

/*
GENERAL TREE SEARCH

    function TREE_SEARCH(problem, strategy) returns a solution, or failure
    initialize the search tree using the initial state of problem
    loop do
        if there are no candidates for expansion then return failure
        if the node contains a goal state then return the corresponding solution
        else expand the node and add the resulting nodes to the search tree
    end

    
*/

// (a) A depth-first search from S to G, given that the order of the operators you will test
// is: up, left, right, then down.  




bool dfs(std::vector<std::string> grid, std::stack<std::array<int, 2>> fringe, std::vector<std::array<int, 2>> path) {
    bool discovered_children = false;

    // Access and pop current coordinates
    std::array<int, 2> coordinates = fringe.top();
    int x = coordinates[0]; int y = coordinates[1];
    fringe.pop();
    path.push_back(coordinates);

    // If goal state, print solution path and return true
    if (grid[y].substr(x, 1) == GOAL_SQUARE) {
        for (std::array<int, 2> xy: path) {
            std::cout << xy[0] << " " << xy[1] << std::endl;
        }
        return true;
    }

    // Check up, left, down, right, to see if they are valid and haven't been visited, then push to stack


    if (valid_square(x, y+1, grid) && !check_coordinate_history(x, y+1, path)) { // Up
        fringe.push({x, y+1});
        discovered_children = true;
    }
    if (valid_square(x-1, y, grid) && !check_coordinate_history(x-1, y, path)) { // Left
        fringe.push({x-1, y});
        discovered_children = true;
    }
    if (valid_square(x+1, y, grid) && !check_coordinate_history(x+1, y, path)) { // Right
        fringe.push({x+1, y});
        discovered_children = true;
    }
    if (valid_square(x, y-1, grid) && !check_coordinate_history(x, y-1, path)) { // Down
        fringe.push({x, y-1});
        discovered_children = true;
    }
    
    // If we didn't add any children, go up a node (pop back of path history)
    if (!discovered_children) {path.pop_back();}

    // If fringe is empty after adding all valid children, return false
    if(fringe.empty()) {return false;}

    // Iterate on next value of the stack
    return dfs(grid, fringe, path);
}

bool check_coordinate_history(int x, int y, std::vector<std::array<int, 2>> history) {
    std::array<int, 2> coordinates = {x, y};
    auto it = std::find(history.begin(), history.end(), coordinates);
    if (it == history.end()) {return false;}
    else {return true;}
}