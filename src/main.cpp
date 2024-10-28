#include <planner.hpp>
#include <iostream>
#include <memory>
#include <queue>

int main(int argc, char** argv) {
    Position start = {0, 0};
    Position goal = {4, 4};
    std::vector<std::vector<int>> grid = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0}
    };

    auto planner = std::make_shared<AStarPlanner>(grid);
    Path path = planner->plan(start, goal);

    for (auto pos : path) {
        std::cout << "(" << pos.first << ", " << pos.second << ")" << std::endl;
    }

    return 0;
}