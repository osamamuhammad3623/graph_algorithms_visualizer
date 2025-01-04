#ifndef BFS_GRAPH_ALGORITHM_H
#define BFS_GRAPH_ALGORITHM_H

#include "graph_algorithm.h"
#include <queue>

class bfs_graph_algorithm : public graph_algorithm
{
public:
    int execute() override;

private:
    std::queue<QGraphicsRectItem*> bfs_queue;
};

#endif // BFS_GRAPH_ALGORITHM_H
