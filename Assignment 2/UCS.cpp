/**
* @author John S. Anvik (j.anvik@uleth.ca)
* @date 2026-02-06
*/
/* You are given a grid similar to the one above. You may prompt the user to enter the width and the height of the grid, the start and the goal states, and the forbidden squares.  
Manhattan distance should be used as the heuristic function, and ties are solved using up, left, right, and down.  
Implement each of the following searches and show the solutions:  
**(c) UCS search from S to G.**  */

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <array>
#include <limits>
#include <cmath>

// Characters to represent grid squares
bool entering_forbidden_squares = true;
const std::string EMPTY_SQUARE = "-";
const std::string STARTING_SQUARE = "S";
const std::string GOAL_SQUARE = "G";
const std::string FORBIDDEN_SQUARE = "X";

class Coordinates {
  private:
    int x;
    int y;
    Coordinates* parent;
    Coordinates* goal;
    int direction; // What direction did you go to get to this square? 1 = up, 2 = left, 3 = right, 4 = down. Used for breaking ties
  public:
    // Constructors
    Coordinates(int xcoord, int ycoord) {
        x = xcoord;
        y = ycoord;
        parent = nullptr;
        goal = nullptr;
    }
    Coordinates(int xcoord, int ycoord, Coordinates* p, Coordinates* g, int d) {
        x = xcoord;
        y = ycoord;
        parent = p;
        goal = g;
        direction = d;
    }
    int getx() {return x;}
    int gety() {return y;}
    void setParent(Coordinates* p) {parent = p;}
    Coordinates* getParent() {return parent;}
    void setGoal(Coordinates* g) {goal = g;}
    Coordinates* getGoal() {return goal;}
    int getDirection() {return direction;}
    // g(n) (Backwards cost)
    int getDepth() {
        // std::cout << "DEPTH" << std::endl;
        if (parent == nullptr) {return 0;}
        else {return 1 + parent->getDepth();}
    }
    friend bool operator>(Coordinates a, Coordinates b);
    friend bool operator<(Coordinates a, Coordinates b);
    friend bool operator==(Coordinates a, Coordinates b);
    friend bool operator!=(Coordinates a, Coordinates b);
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
/**
 * @brief returns depth of a state in the search tree
 * @param current current state node
 */
int check_coordinate_depth(Coordinates* current);


/**
 * @brief defining comparison operators for coordinates to give highest priority to those closest to the goal that was just defined
 */
bool operator!=(Coordinates a, Coordinates b) {
    return(a.getDepth() != b.getDepth());
}
bool operator==(Coordinates a, Coordinates b) {
    return(a.getDepth() == b.getDepth());
}
 bool operator>(Coordinates a, Coordinates b) {
    if (a.getDepth() == b.getDepth()) {return a.getDirection() < b.getDirection();} // To break ties
    return(a.getDepth() > b.getDepth());
}
bool operator<(Coordinates a, Coordinates b) {
    if (a.getDepth() == b.getDepth()) {return a.getDirection() > b.getDirection();} // To break ties
    return(a.getDepth() < b.getDepth());
}


/**
 * @brief searches by exploring nodes with smallest value of backwards cost first
 * @param grid
 * @param fringe
 */
bool ucssearch(std::vector<std::string> grid, std::priority_queue<Coordinates*, std::vector<Coordinates*>, std::greater<Coordinates*>> fringe, Coordinates* goal);


int main() {
    int x, y;
    std::vector<std::string> grid = {};
    std::priority_queue<Coordinates*, std::vector<Coordinates*>, std::greater<Coordinates*>> starting_queue; // Priority queue for search fringe
    std::vector<Coordinates*> history; // Vector used to store current path
    Coordinates* start;

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
            start = new Coordinates(x, y);
            starting_queue.push(start);
        } else {std::cout << "INVALID STARTING SQUARE\n";}
    }
    print_grid(grid);
    x = 0; y = 0; // Reset x and y to invalid coordinates

    // Add goal square to grid
    while (!valid_square(x, y, grid) || grid[y].substr(x, 1) == STARTING_SQUARE) {
        std::cout << "\n Enter goal coordinates in the following form: x y ";
        std::cin >> x >> y; 
        // Check for valid goal coordinates, then place goal square
        if (valid_square(x, y, grid) && grid[y].substr(x, 1) != STARTING_SQUARE) {
            Coordinates* goal = new Coordinates(x, y);
            grid[y].replace(x, 1, GOAL_SQUARE);
            start->setGoal(goal);
        } else {std::cout << "INVALID GOAL SQUARE\n";}
    }
    print_grid(grid);
    x = 0; y = 0; // Reset x and y to invalid coordinates

    // Enter forbidden squares
    while (entering_forbidden_squares) {
        std::cout << "\n To place a forbidden square, enter coordinates in the following form: x y \n or enter -1 -1 to place no more forbidden squares \n";
        std::cin >> x >> y;
        if (x !=-1 && y!=-1) {
            if (valid_square(x, y, grid) && (grid[y].substr(x, 1) != STARTING_SQUARE) && (grid[y].substr(x, 1) != GOAL_SQUARE)) {
                grid[y].replace(x, 1, FORBIDDEN_SQUARE);
                print_grid(grid);
            } else {
                std::cout << "INVALID SQUARE" << std::endl;
            }
        } else {entering_forbidden_squares = false;}
    }

    x = 0; y = 0; // Reset x and y to invalid coordinates

    if(!ucssearch(grid, starting_queue, start->getGoal())) {std::cout << "NO VALID SOLUTION\n";}

    return 0;
}


bool valid_square(int x, int y, std::vector<std::string> g) {
    return (x > 0 && y > 0 && y < g.size() && x < g.at(0).length() && g[y].substr(x, 1) != FORBIDDEN_SQUARE);
}


void print_grid(std::vector<std::string> g) {
    std::cout << std::endl;
    for (std::string s : g) {std::cout << s << std::endl;}
}

bool ucssearch(std::vector<std::string> grid, std::priority_queue<Coordinates*, std::vector<Coordinates*>, std::greater<Coordinates*>> fringe, Coordinates* goal) {
    bool discovered_children = false; // Did we find children for this node?

    // Access and pop current coordinates
    Coordinates* coordinates = fringe.top();
    // std::cout << coordinates->priority() << std::endl;
    int x = coordinates->getx(); int y = coordinates->gety();
    fringe.pop();
    // If goal state, print solution path and return true
    if (grid[y].substr(x, 1) == GOAL_SQUARE) {
        // TODO: Figure out how to correctly print history
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
    Coordinates* up = new Coordinates(x, y+1, coordinates, goal, 1);
    if (valid_square(up->getx(), up->gety(), grid) && !check_coordinate_history(coordinates, up)) { // Up
        fringe.push(up);
        discovered_children = true;
    }
    Coordinates* left = new Coordinates(x-1, y, coordinates, goal, 2);
    if (valid_square(left->getx(), left->gety(), grid) && !check_coordinate_history(coordinates, left)) { // Left
        fringe.push(left);
        discovered_children = true;
    }
    Coordinates* right = new Coordinates(x+1, y, coordinates, goal, 3);
    if (valid_square(right->getx(), right->gety(), grid) && !check_coordinate_history(coordinates, right)) { // Right
        fringe.push(right);
        discovered_children = true;
    }
    Coordinates* down = new Coordinates(x, y-1, coordinates, goal, 4);
    if (valid_square(down->getx(), down->gety(), grid) && !check_coordinate_history(coordinates, down)) { // Down
        fringe.push(down);
        discovered_children = true;
    }

    // If fringe is empty after adding all valid children, return false
    if(fringe.empty()) {return false;}

    // Iterate on next value of the stack
    return ucssearch(grid, fringe, goal);
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