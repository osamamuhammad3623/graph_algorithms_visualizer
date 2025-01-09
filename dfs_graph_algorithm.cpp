#include "dfs_graph_algorithm.h"

int dfs_graph_algorithm::execute()
{
    dfs_stack.push(map_squares[start_row][start_col]);
    bool finish = false;
    int steps=0;

    // Store the predecessor of each node for path reconstruction
    std::unordered_map<QGraphicsRectItem*, QGraphicsRectItem*> came_from;

    while ((!finish) && (!stop_flag)) {
        QGraphicsRectItem* current = dfs_stack.top();
        map_squares[current->rect().y() / SQUARE_SIZE][current->rect().x() / SQUARE_SIZE]->setBrush(Qt::lightGray);
        dfs_stack.pop();

        // Add neighbours to the stack
        for (auto nei : get_neighbours(current)) {
            int row = nei->rect().y() / SQUARE_SIZE;
            int col = nei->rect().x() / SQUARE_SIZE;

            if (map_squares[row][col]->brush() == Qt::black) {
                continue;
            } else if (map_squares[row][col]->brush() == Qt::green) {
                finish = true;
                came_from[nei] = current;  // Record the predecessor
                break;
            } else if (map_squares[row][col]->brush() == Qt::red) {
                dfs_stack.push(nei);
                came_from[nei] = current;  // Record the predecessor
            }
        }

        finish |= dfs_stack.empty();
        delay();
        steps++;
    }

    if(finish){
        reconstruct_path(came_from);
    }
    return steps;
}
