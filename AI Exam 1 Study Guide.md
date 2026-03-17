# CPSC 3750 Artificial Intelligence Midterm Study Guide
By Colby Hanna  

## First Lecture Slides

### Types of Agents
**Reflex Agents**
- Choose actions based on the current perception and potentially a memory of the worlds current state
- Do not consider the future consequences of their action; only considers how the world is currently  
**Planning Agent**
- Decisions are based on hypothesized consequences
- Must have a model of how the world revolves in response to actions and must formulate a goal test
- they consider how the world would be
- Formulates a test goal  
https://www.youtube.com/watch?v=5XD-Aqp3eRQ  
### Search Problems and State Spaces  
##### Search Problem Components  
- State Space  
- A successor functions with actions and costs  
- start and end goal  
- A solution is a sequence of actions i.e. a plan which transforms the start state to a goal state   
##### State Space Graph  
https://www.youtube.com/watch?v=iZ4QB4a_tZg&pp=ygURU3RhdGUgU3BhY2UgR3JhcGg%3D  
- A mathematical representation where nodes are abstracted world configurations and arcs represent successors. Each state occurs only once in this graph  
- Each state only occurs once
- We can rarely build this full graph in memory as its too big   
- ![[Pasted image 20260301162152.png]]  
##### Search Tree  
- A "What IF" tree of plan and outcomes where the start space is the root, and children are successors. nodes represent plans that achieve certain states  
![[Pasted image 20260301162644.png]]  
![[Pasted image 20260301162728.png]]  
![[Pasted image 20260301163443.png]]  
### Search Algorithms  
##### Depth First Search  
- Strategy - Expand Deepest Node First  
- Implementation - LIFO Stack  
- Complete? - only if m (max depth) is finite  
- Optimal? - no  
- https://www.youtube.com/watch?v=Urx87-NMm6c  
##### Breadth First Search BFS  
- Strategy - Expand Shallowest node first  
- Implementation - FIFO Queue  
- Complete? - Yes  
- Optimal? - Only if all costs are 1  
- https://www.youtube.com/watch?v=HZ5YTanv5QE  

##### Uniform Cost Search UCS  
- Strategy - Expand Cheapest node first  
- Implementation - Priority Queue Cumulative cost  
- Complete? - Yes is cost is > 0  
- Optimal? - Yes  
- https://www.youtube.com/watch?v=dRMvK76xQJI  
### Search Complexity and Properties  
- **Branching Factor ($b$)**: The number of successors for a node.  
- **DFS Complexity**: Time $O(b^m)$, Space $O(bm)$.  
- **BFS Complexity**: Time $O(b^s)$, Space $O(b^s)$, where $s$ is the depth of the shallowest solution.  
- **UCS Complexity**: Time and Space $O(b^{C^*/\epsilon})$, where $C^*$ is the cost of the optimal solution and $\epsilon$ is the minimum arc cost.  

## SECOND SLIDE  

### Informed vs Uninformed Search  
- **Uninformed Search (Blind Search)** - Uses only the information available in the problem definition ( e.g. BFS, DFS, UCS). They Don't know if they are getting close to the goal  
- **Informed Search (Heuristic Search)** - Uses domain specific hints to guide the search toward the goal more efficiently  

### Search heuristic  
- **Definition:** A function $h(s)$ that estimates the cost from state $s$ to the nearest goal.  
    
- **Characteristics:**  
    - Designed for specific problems (e.g., pathfinding).  
    - $h(goal) = 0$.  
- **Examples for Pathfinding:**  
    - **Manhattan Distance:** Sum of horizontal and vertical distances (ideal for grid-based movement). ( |x1 - x2| + |y1 - y2|)  
    - **Euclidean Distance:** "As the crow flies" straight-line distance.  
![[Pasted image 20260301180128.png]]  

### Greedy Search   
- Strategy - Expand the node that seems closest to the goal lowest h(n)  
- flaw it ignores the cost spent to get to the current node g(n)  
- properites:  
	- Not optimal it can take a long, expensivve just because it looks "direct" at the start  
	- Not Complete: Can get stuck in loops (in Tree Search)  
- https://www.youtube.com/watch?v=GVvN0ikNekw  

### A* Search  
- A* Combines the cost to reach a node and the estimated cost to the goal  
	- Formula: F(n) = g(n) + h(n)  
		- g(n) Cost to reach node n (backwards cost)  
		- h(n) estimated cost from n to the goal (forward cost)  
		- f(n) Estimated total cost of a path through n  
- https://www.youtube.com/watch?v=vP5TkF0xJgI  
##### Conditions for Optimality  
To Guarantee A* finds the cheapest path, the heuristic must meet certain criteria:  
1. Admissibility (required for Tree Search):  
- A heuristic is admissible if it never overestimates the true cost to the goal  
- $0 \leq h(n) \leq h^*(n)$, where $h^*(n)$ is the true optimal cost.  
2. Consistency (required for Graph Search)  
- The Estimate h(n) must be "smooth". The change in h between two nodes should not be greater than the actual cost of the edge between them
- Cost( A -> C) >= h(A) - h(C)  
- Consistency is a stronger requirement than admissibility  

### Graph Search vs Tree Search  
- Tree Search: Potential for redundant work; can explore the same state multiple times if reached via different paths  
- Graph Search Keeps a "closed set" (or explored set) to ensure each state is expanded only once  
	- Never expands a state that has been expanded before  
	- for Graph Search to remain optimal the heuristic must be consistent  
### Heuristic Design
- Relaxed Problems A common way to create admissible heuristic. You remove constraints from the original problem (e.g. teleportation or moving through walls). The cost of an optimal solution to a relaxed problem is an admissible heuristic for the original problem  
- Dominance if h2(n) >= h1(n) for all n, then h2 is dominant and is generally "better" because it provided a more accurate estimate (closer to the true cost) and will expand fewer nodes  

|**Algorithm**|**Fringe Priority**|**Goal**|
|---|---|---|
|**UCS**|$g(n)$|Find cheapest path|
|**Greedy**|$h(n)$|Find goal quickly|
|**A***|$g(n) + h(n)$|Find cheapest path efficiently|

## Third Lecture Slides GAMES  

### Types of Games  
Ai Agents operate in environments with other agents, games can be categorized by several factors  
- **Deterministic** vs **Stochastic**: Is the outcome of an action certain (Chess) or involve randomness/dice (Monopoly)  
- **Perfect vs Imperfect Information**: Can you see the entire state (Checkers) or is some information hidden (Poker)  
- **Zero-Sum**: One Players gain is the other's loss. The total utility always sums to zero (pure competition)  
- **General Games**: Players have independent utilities; can involve cooperation or indifference  
### Minimax Search
	Used in deterministic zeo-sumn perfect information games between two players, MAX tries to maximize the score and MIN the opponent tries to minimize the score  
- The Goal: Find the best move for MAX, assuming MIN plays optimally  
- The Process:  
	1. Generate the full game tree down to the terminal end states  
	2. Assign scores to terminal states  
	3. Back up values  
	- * **Min nodes** take the minimum of their children's values. *   
	- **Max nodes** take the maximum of their children's values.  
![[Pasted image 20260302135431.png]]  
![[Pasted image 20260302135501.png]]  
### Alpha Beta Pruning
Minimax explores many unnecessary branches. Alpha beta pruning allows us to skip (prune) branches the cannot possibly affect the final decision
- **Alpha** - The best value MAX can guarantee so far  
- **Beta** The best value MIN can guarantee so far  
- **Pruning Rule:** If at any point we find a branch where a value is worse than a previously discovered guarantee ($\alpha \ge \beta$), we stop searching that branch.  
- **Efficiency:** Perfect ordering of moves allows searching twice as deep ($O(b^{m/2})$ instead of $O(b^m)$).  
- https://www.youtube.com/watch?v=l-hh51ncgDI  
### Resource Limits and Evaluation Functions  
In complex games like Chess, the tree is too large to reach terminal states.  
- **Depth-Limited Search:** Search only a few "plies" (turns) deep.  
- **Evaluation Function ($V(s)$):** Estimates how "good" a non-terminal state is.  
    - Example: In Chess, $V(s) = (\text{w. pieces}) - (\text{b. pieces})$.  
    - A better evaluation function allows for shallower, more accurate searches.  