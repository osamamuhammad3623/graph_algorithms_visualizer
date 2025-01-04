#ifndef A_STAR_GRAPH_ALGORITHM_H
#define A_STAR_GRAPH_ALGORITHM_H
#include <QGraphicsRectItem>
#include "graph_algorithm.h"
#include <queue>

struct Node {
    QGraphicsRectItem* item;   // The graphical square (node)
    int g_cost;                // Cost from the start node
    int h_cost;                // Heuristic cost (estimated cost to the goal)
    int f_cost() const { return g_cost + h_cost; }  // Total cost function

    // For priority queue comparison (min-heap based on f_cost)
    bool operator>(const Node& other) const {
        return f_cost() > other.f_cost();
    }
};


class a_star_graph_algorithm : public graph_algorithm
{
public:
    a_star_graph_algorithm(bool calculate_heuristics);
    int execute() override;

private:    
    bool calculate_heuristics;
    int calculate_heuristic(QGraphicsRectItem *current, QGraphicsRectItem *goal);

};

#endif // A_STAR_GRAPH_ALGORITHM_H
