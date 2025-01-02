#ifndef A_STAR_NODE_H
#define A_STAR_NODE_H
#include <QGraphicsRectItem>


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

#endif // A_STAR_NODE_H
