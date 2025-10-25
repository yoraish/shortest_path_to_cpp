#include <vector>
#include <iostream>
#include <memory>
#include <queue>
#include <unordered_set>

using Position = std::pair<int, int>;
using Path = std::vector<Position>;

// Custom hash function for std::pair<int, int>
namespace std {
    template <>
    struct hash<std::pair<int, int>> {
        std::size_t operator()(const std::pair<int, int>& p) const {
            auto hash1 = std::hash<int>{}(p.first);
            auto hash2 = std::hash<int>{}(p.second);
            return hash1 ^ (hash2 << 1);
        }
    };
}

struct SearchState {
    int g;
    int h;
    int f;
    Position pos;
    std::shared_ptr<SearchState> parent;
};

using SearchStatePtr = std::shared_ptr<SearchState>;

// Comparator for search state pointer.
struct SearchStatePtrComparator {
    bool operator()(const SearchStatePtr& lhs, const SearchStatePtr& rhs) const {
        // Returns true if lhs should be popped before rhs.
        return lhs->f > rhs->f;
    }
};

class Planner {
public:
    virtual Path plan(Position start, Position goal)=0;
    
private:
};

class AStarPlanner : public Planner {
private:
    // Variables.
    std::vector<std::vector<int>> grid_;
    std::priority_queue<SearchStatePtr, std::vector<SearchStatePtr>, SearchStatePtrComparator> open_;
    std::unordered_set<Position> closed_;
    
    // Methods.
    int computeHeuristic(Position a, Position b);
    std::vector<SearchStatePtr> getSuccessors(const SearchStatePtr& state);

public:
    AStarPlanner(const std::vector<std::vector<int>>& grid);
    ~AStarPlanner();
    Path plan(Position start, Position goal) override;
};