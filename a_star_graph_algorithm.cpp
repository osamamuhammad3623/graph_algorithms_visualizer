#include "a_star_graph_algorithm.h"

a_star_graph_algorithm::a_star_graph_algorithm(bool calculate_heuristics)
{
    this->calculate_heuristics = calculate_heuristics;
}

int a_star_graph_algorithm::execute()
{
    // Initialize the start and goal positions
    QGraphicsRectItem* start = map_squares[0][0];  // Example start position
    int steps=0;

    // The std::priority_queue is used to process nodes in the order of their f-cost,
    //      which ensures that we explore the least costly paths first.
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open_list;
    std::unordered_map<QGraphicsRectItem*, QGraphicsRectItem*> came_from;  // For path reconstruction
    std::unordered_map<QGraphicsRectItem*, int> g_cost_map;  // Store g_cost for each node

    // Initialize the start node
    if(calculate_heuristics){
        open_list.push(Node{start, 0, calculate_heuristic(start, target_square)});
    }else{
        open_list.push(Node{start, 0, 0});
    }

    g_cost_map[start] = 0;

    bool finish = false;

    while ((!open_list.empty()) && (!finish) && (!stop_flag)) {
        // Get the node with the lowest f_cost
        Node current_node = open_list.top();
        open_list.pop();

        QGraphicsRectItem* current = current_node.item;
        int row = current->rect().y() / SQUARE_SIZE;
        int col = current->rect().x() / SQUARE_SIZE;

        // Mark the current node as visited
        map_squares[row][col]->setBrush(Qt::lightGray);

        if (current == target_square) {
            finish = true;
            break;
        }

        // Process the neighbors of the current node
        for (auto nei: get_neighbours(current)) {
            row = nei->rect().y() / SQUARE_SIZE;
            col = nei->rect().x() / SQUARE_SIZE;

            if (map_squares[row][col]->brush() == Qt::black) {  // Skip blocked nodes
                continue;
            }

            int tentative_g_cost = g_cost_map[current] + 1;  // Assuming uniform cost for simplicity

            int h_cost=0;
            if(calculate_heuristics){
                h_cost = calculate_heuristic(nei, target_square);
            }

            // If this path is better, or the neighbor is not in the open list yet
            if (g_cost_map.find(nei) == g_cost_map.end() || tentative_g_cost < g_cost_map[nei]) {
                g_cost_map[nei] = tentative_g_cost;
                open_list.push(Node{nei, tentative_g_cost, h_cost});
                came_from[nei] = current;  // Store the path for reconstruction
            }
        }

        delay();
        steps++;
    }

    if(finish){
        reconstruct_path(came_from);
    }

    return steps;
}

int a_star_graph_algorithm::calculate_heuristic(QGraphicsRectItem *current, QGraphicsRectItem *goal)
{
    int current_row = current->rect().y() / SQUARE_SIZE;
    int current_col = current->rect().x() / SQUARE_SIZE;
    int goal_row = goal->rect().y() / SQUARE_SIZE;
    int goal_col = goal->rect().x() / SQUARE_SIZE;
    // Manhattan distance heuristic
    return std::abs(current_row - goal_row) + std::abs(current_col - goal_col);
}
