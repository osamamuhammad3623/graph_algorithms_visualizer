#include "bfs_graph_algorithm.h"

int bfs_graph_algorithm::execute()
{
    bfs_queue.push(map_squares[start_row][start_col]);
    bool finish = false;
    int steps=0;

    // Store the predecessor of each node for path reconstruction
    std::unordered_map<QGraphicsRectItem*, QGraphicsRectItem*> came_from;

    while ((!finish) && (!stop_flag)) {
        // add neighbours
        for (auto nei : get_neighbours(bfs_queue.front())) {
            int row = nei->rect().y() / SQUARE_SIZE;
            int col = nei->rect().x() / SQUARE_SIZE;

            if (map_squares[row][col]->brush() == Qt::black) {
                continue;
            } else if (map_squares[row][col]->brush() == Qt::green) {
                finish |= true;
                came_from[nei] = bfs_queue.front();  // Record the predecessor
                break;
            } else if (map_squares[row][col]->brush() == Qt::red) {
                map_squares[row][col]->setBrush(Qt::lightGray);
                came_from[nei] = bfs_queue.front();  // Record the predecessor
                bfs_queue.push(nei);
            }
        }

        bfs_queue.pop();
        finish |= bfs_queue.empty();
        delay();
        steps++;
    }

    if(finish){
        reconstruct_path(came_from);
    }
    return steps;
}

