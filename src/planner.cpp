#include <planner.hpp>
#include <functional>


AStarPlanner::AStarPlanner(const std::vector<std::vector<int>>& grid) : grid_(grid) {
    std::cout << "AStarPlanner constructor called." << std::endl;
}

AStarPlanner::~AStarPlanner() {
    std::cout << "AStarPlanner destructor called." << std::endl;
}

int AStarPlanner::computeHeuristic(Position a, Position b) {
    // Manhattan distance.
    return abs(a.first - b.first) + abs(a.second - b.second);
}

std::vector<SearchStatePtr> AStarPlanner::getSuccessors(const SearchStatePtr& state) {
    std::vector<SearchStatePtr> successors;

    // Define the possible moves.
    std::vector<Position> moves = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}
    };

    // For each move.
    for (auto move : moves) {
        // Compute the new position.
        Position pos_new = {state->pos.first + move.first, state->pos.second + move.second};

        // Check if the new position is within the grid.
        if (pos_new.first < 0 || pos_new.first >= grid_.size() || pos_new.second < 0 || pos_new.second >= grid_[0].size()) {
            continue;
        }

        // Check if the new position is an obstacle.
        if (grid_[pos_new.first][pos_new.second] == 1) {
            continue;
        }

        // Create the new state.
        auto state_new = std::make_shared<SearchState>();
        state_new->pos = pos_new;
        successors.push_back(state_new);
    }

    return successors;
}

Path AStarPlanner::plan(Position start, Position goal) {
    std::cout << "Plan() called" << std::endl;
    // Create the start state.
    auto startState = std::make_shared<SearchState>();
    startState->g = 0;
    startState->h = computeHeuristic(start, goal);
    startState->f = startState->g + startState->h;
    startState->pos = start;
    startState->parent = nullptr;

    // Add the start state to the open list.
    open_.push(startState);

    // While the open list is not empty.
    while (!open_.empty()) {
        // Get the state with the minimum f value.
        auto currentState = open_.top();
        open_.pop();

        // Check if the current state is the goal state.
        if (currentState->pos == goal) {
            // Reconstruct the path.
            Path path;
            while (currentState != nullptr) {
                path.push_back(currentState->pos);
                currentState = currentState->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Add the current state to the closed list.
        closed_.insert(currentState->pos);

        // Get the successors of the current state.
        auto successors = getSuccessors(currentState);

        // For each successor.
        for (auto successor : successors) {
            // If the successor is in the closed list, skip it.
            if (closed_.find(successor->pos) != closed_.end()) {
                continue;
            }

            // Compute the g, h, and f values.
            successor->g = currentState->g + 1;
            successor->h = computeHeuristic(successor->pos, goal);
            successor->f = successor->g + successor->h;
            successor->parent = currentState;

            // Add the successor to the open list.
            open_.push(successor);
        }
    }

    return {};

}