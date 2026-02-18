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
/**
 * @brief returns depth of a state in the search tree
 * @param current current state node
 */
int check_coordinate_depth(Coordinates* current);
/**
 * @brief returns manhattan distance between a pair of coordinates on the grid
 * @param first first square on the grid
 * @param second second square on the grid
 */
int manhattan_distance(Coordinates* first, Coordinates* second)

// Search strategies
bool greedySearch(std::vector<std::string> grid, std::stack<Coordinates*> fringe, int depth_limit);


int main() {
    int x, y;
    std::vector<std::string> grid = {};
    std::stack<Coordinates*> starting_stack; // Stack used for DFS fringe
    std::priority_queue<Coordinates*> starting_queue; // Stack used for DFS fringe
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
    
    // Add starting squa