/**
* @author John S. Anvik (j.anvik@uleth.ca)
* @date 2026-02-06
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
#include <queue>
#include <array>
#include <limits>

// CHaracters to represent grid squares
bool entering_forbidden_squares = true;
const std::string EMPTY_SQUARE = "-";
const std::string STARTING_SQUARE = "S";
const std::string GOAL_SQUARE = "G";
const std::string FORBIDDEN_SQUARE = "X";
const unsigned int INFINITY = std::numeric_limits<int>::max(); // Used for DFS with no depth limit

class Coordinates {
  private:
    int x;
    int y;
    Coordinates* parent;
  public:
    // Constructors
    Coordinates(int xcoord, int ycoord) {
        x = xcoord;
        y = ycoord;
        parent = nullptr;
    }
    Coordinates(int xcoord, int ycoord, Coordinates* p) {
        x = xcoord;
        y = ycoord;
        parent = p;
    }
    int getx() {return x;}
    int gety() {return y;}
    void setParent(Coordinates* p) {parent = p;}
    Coordinates* getParent() {return parent;}
};

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

/**
 * @brief recursively searches all parents of a point to see if another point is in there. Used to avoid infinite loops
 * @param current coordinate whose history we want to check
 * @param tosearch coordinate we want to look for in history
 * @return returns true if found tosearch in history, else returns false
 */
bool check_coordinate_history(Coordinates* current, Coordinates* tosearch);
int check_coordinate_depth(Coordinates* current);

// Search strategies
bool dfs(std::vector<std::string> grid, std::stack<Coordinates*> fringe, int depth_limit);

bool idfs(std::vector<std::string> grid, std::stack<Coordinates*> fringe, int depth_limit);


int main() {
    int x, y;
    std::vector<std::string> grid = {};
    std::stack<Coordinates*> starting_stack; // Stack used for DFS fringe
    std::queue<Coordinates*> starting_queue; // Stack used for DFS fringe
    std::vector<Coordinates*> history; // Vector used to store current path
    
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
            grid[y].replace(x, y, STARTING_SQUARE);
            Coordinates* start = new Coordinates(x, y);
            starting_stack.push(start);
            starting_queue.push(start);
        } else {std::cout << "INVALID STARTING SQUARE\n";}
    }
    print_grid(grid);
    x = 0; y = 0; // Reset x and y to invalid coordinates

    while (!valid_square(x, y, grid)) {
        std::cout << "\n Enter goal coordinates in the following form: x y ";
        std::cin >> x >> y; 
        // Check for valid goal coordinates, then place goal square
        if (valid_square(x, y, grid)) {
            grid[y].replace(x, y, GOAL_SQUARE);
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

    idfs(grid, starting_stack, 1);

    return 0;
}


bool valid_square(int x, int y, std::vector<std::string> g) {
    return (x > 0 && y > 0 && y < g.size() && x < g.at(0).length() && g[y].substr(x, 1) != FORBIDDEN_SQUARE);
}


void print_grid(std::vector<std::string> g) {
    std::cout << std::endl;
    for (std::string s : g) {std::cout << s << std::endl;}
}



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

// (a) A depth-first search from S to G, given that the order of the operators you will test is: up, left, right, then down.  


bool dfs(std::vector<std::string> grid, std::stack<Coordinates*> fringe, int depth_limit = INFINITY) {
    bool discovered_children = false; // Did we find children for this node?

    // Access and pop current coordinates
    Coordinates* coordinates = fringe.top();
    int x = coordinates->getx(); int y = coordinates->gety();
    fringe.pop();

    // If goal state, print solution path and return true
    if (grid[y].substr(x, 1) == GOAL_SQUARE) {
        // Store history in vector backwards, then print forwards
        std::vector<Coordinates*> history;
        while(coordinates != nullptr) {
            history.push_back(coordinates);
            coordinates = coordinates->getParent();
        }
        for (int i = history.size()-1; i >= 0; --i) {        
            std::cout << "(" << history[i]->getx() << ", " << history[i]->gety() << "), ";
        }
        std::cout << std::endl;
        return true;
    }

    // Check up, left, down, right, to see if they are valid and haven't been visited, then push to stack

    Coordinates* up = new Coordinates(x, y+1, coordinates);
    if (valid_square(up->getx(), up->gety(), grid) && !check_coordinate_history(coordinates, up) && check_coordinate_depth(coordinates) < depth_limit) { // Up
        fringe.push(up);
        discovered_children = true;
    }
    Coordinates* left = new Coordinates(x-1, y, coordinates);
    if (valid_square(left->getx(), left->gety(), grid) && !check_coordinate_history(coordinates, left) && check_coordinate_depth(coordinates) < depth_limit) { // Left
        fringe.push(left);
        discovered_children = true;
    }
    Coordinates* right = new Coordinates(x+1, y, coordinates);
    if (valid_square(right->getx(), right->gety(), grid) && !check_coordinate_history(coordinates, right) && check_coordinate_depth(coordinates) < depth_limit) { // Right
        fringe.push(right);
        discovered_children = true;
    }
    Coordinates* down = new Coordinates(x, y-1, coordinates);
    if (valid_square(down->getx(), down->gety(), grid) && !check_coordinate_history(coordinates, down) && check_coordinate_depth(coordinates) < depth_limit) { // Down
        fringe.push(down);
        discovered_children = true;
    }

    // If fringe is empty after adding all valid children, return false
    if(fringe.empty()) {return false;}

    // Iterate on next value of the stack
    return dfs(grid, fringe, depth_limit);
}

// (c) An iterative deepening depth-first search from S to G, given that the order of the operators you will test is: up, left, right, then down. 
// At what depth is the solution reached?  

bool idfs(std::vector<std::string> grid, std::stack<Coordinates*> fringe, int depth_limit) {
    std::stack<Coordinates*> starting_stack; // Stack used for DFS fringe
    starting_stack.push(fringe.top());
    std::vector<Coordinates> history; // Starting vector for each iteration of DFS
    if (depth_limit >= INFINITY) {return false;}
    else if (dfs(grid, fringe, depth_limit)) {
        std::cout << "SOLUTION DEPTH: " << depth_limit << std::endl;
        return true;
    }
    else {return idfs(grid, starting_stack, depth_limit+1);}
}

bool check_coordinate_history(Coordinates* current, Coordinates* tosearch) {
    while(current->getParent() != nullptr) {
        if (current->getParent()->getx() == tosearch->getx() && current->getParent()->gety() == tosearch->gety()) {
            // std::cout << "FOUND IN HISTORY" << std::endl;
            return true;
        }
        else {return check_coordinate_history(current->getParent(), tosearch);}
    }
    return false;
}

int check_coordinate_depth(Coordinates* current) {
    if (current->getParent() == nullptr) {return 0;}
    else {return 1 + check_coordinate_depth(current->getParent());}
}