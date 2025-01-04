#ifndef DFS_GRAPH_ALGORITHM_H
#define DFS_GRAPH_ALGORITHM_H

#include "graph_algorithm.h"
#include <stack>

class dfs_graph_algorithm : public graph_algorithm
{
public:
    int execute() override;

private:
    std::stack<QGraphicsRectItem*> dfs_stack;
};

#endif // DFS_GRAPH_ALGORITHM_H
